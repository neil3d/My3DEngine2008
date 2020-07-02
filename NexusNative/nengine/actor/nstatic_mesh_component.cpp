#include "StdAfx.h"
#include "nstatic_mesh_component.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../physics/nphysics_framework.h"
#include "../physics/nphys_user_stream.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nstatic_mesh_component, nmesh_component)

	nstatic_mesh_component::nstatic_mesh_component(const nstring& name_str)
		:nmesh_component(name_str)
	{
		nresource_manager::instance()->register_event_target(this);
		m_clone_mtl = true;
	}

	nstatic_mesh_component::~nstatic_mesh_component(void)
	{	
		nresource_manager::instance()->unregister_event_target(this);
	}

	void nstatic_mesh_component::reset_resource(nresource_static_mesh::ptr mesh_ptr)
	{
		m_static_mesh = mesh_ptr;
		
		if( m_static_mesh
			&& m_static_mesh->ready() )
			on_resource_ready();
	}

	nmtl_base* nstatic_mesh_component::get_material(int lod, int mtl_id)
	{
		//-- 从基类的Replace Mtl中查找
		nmtl_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		//-- 从自身的资源材质克隆中查找
		std::pair<int, int> mtl_key(lod, mtl_id);
		material_map::iterator iter = m_res_mtl_clone.find( mtl_key );
		if(iter != m_res_mtl_clone.end())
			return iter->second.get();

		//-- 使用资源自身的材质（共享）
		nmtl_base::ptr res_mtl;
		if( m_static_mesh
			&& m_static_mesh->ready() )
		{
			res_mtl = m_static_mesh->get_material(lod, mtl_id);			
		}
		if( res_mtl)
			return res_mtl.get();
		else		
			return nresource_manager::instance()->get_default_material();		
	}

	void nstatic_mesh_component::_update_transform(const object_space& parent_space)
	{
		nmesh_component::_update_transform(parent_space);

		if( m_static_mesh
			&& m_static_mesh->ready() )
		{
			m_bounds = m_static_mesh->get_bounds();
			m_bounds.transform_by( get_world_matrix() );
		}

		nscene_event evt(ESA_MovePrimitive, this);
		m_owner->on_event(evt);
	}

	void nstatic_mesh_component::draw_simple_mesh(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		if( m_static_mesh
			&& m_static_mesh->ready() )
		{
			PDI->draw_simple_mesh( m_static_mesh->get_render_simple_mesh(), color );
		}
	}

	void nstatic_mesh_component::_destroy()
	{
		m_static_mesh.reset();
		nmesh_component::_destroy();
	}

	void nstatic_mesh_component::serialize(narchive& ar)
	{
		nmesh_component::serialize(ar);

		resource_location static_mesh_loc;
		if( m_static_mesh )
			static_mesh_loc = m_static_mesh->get_location();

		nSERIALIZE(ar, static_mesh_loc);

		if( ar.is_loading() )
		{
			reset_resource( static_mesh_loc );
		}
	}

	void nstatic_mesh_component::reset_resource(const resource_location& static_mesh_loc, enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		nresource_static_mesh::ptr mesh_ptr(
			nresource_manager::instance()->load_static_mesh(static_mesh_loc, mode, pri)
			);

		reset_resource(mesh_ptr);
	}

	void nstatic_mesh_component::_on_device_lost(int param)
	{
		m_static_mesh->_on_device_lost(param);
	}

	bool nstatic_mesh_component::_on_device_reset(int param)
	{
		(void)param;
		m_static_mesh->_on_device_reset(param);
		on_resource_ready();
		return true;
	}

	void nstatic_mesh_component::_init_cmp_phys()
	{
		if( m_owner==NULL || m_static_mesh.get()==NULL )	
		{ return;}

		// 不产生任何碰撞
		if( m_disable_all_rigidbody )
		{
			return;
		}

		NxPhysicsSDK* nx_sdk = nengine::instance()->get_phys_engine()->get_nx_physics_sdk();
		if( nx_sdk == NULL ) return;

		nlevel* level_owner = m_owner->get_owner();
		if( level_owner == NULL )return;
		nphys_scene* phys_scene = level_owner->get_phys_scene();
		if( phys_scene == NULL || !phys_scene->is_valid_scene() ) return;
		NxScene* nx_scene = phys_scene->get_nx_scene();

		// Create descriptor for convex mesh
		NxActorDesc StaticMeshActorDesc;
		if( m_static_mesh->is_pretriangle_collision() )
		{
			std::vector<NxVec3>			vertex_buffer;
			std::vector<unsigned short>	index_buffer;
			nstatic_mesh_lod::ptr lod_mesh = m_static_mesh->get_lod_mesh(0) ;
			vertex_stream* pos_stream = lod_mesh->m_vert_data->get_stream(0);
			for(size_t i=0; i<lod_mesh->m_secton_array.size(); i++)
			{
				nmesh_section::ptr sec = lod_mesh->m_secton_array[i];

				for(size_t j=0; j<sec->m_index_buffer.get_index_count(); j++)
				{
					index_buffer.push_back( sec->m_index_buffer.data[j] );
				}
			}

			vector3* src_verts = (vector3*)pos_stream->get_data();
			for ( size_t i=0; i<pos_stream->get_num_vert(); i++ )
			{
				//@ todo component scale
				vertex_buffer.push_back( physx::nx_vector_conv(src_verts[i]) );
			}

			// Create PhysX mesh descriptor and fill it in.
			NxTriangleMeshDesc StaticMeshDesc;

			StaticMeshDesc.numVertices = pos_stream->get_num_vert();
			StaticMeshDesc.pointStrideBytes = sizeof(NxVec3);
			StaticMeshDesc.points = &(*vertex_buffer.begin());

			StaticMeshDesc.numTriangles = index_buffer.size()/3;
			StaticMeshDesc.triangleStrideBytes = 3*sizeof(unsigned short);
			StaticMeshDesc.triangles = &(*index_buffer.begin());

			StaticMeshDesc.flags = NX_MF_16_BIT_INDICES;
			StaticMeshDesc.flags |= NX_MF_FLIPNORMALS;

			if( NxGetCookingParams().targetPlatform == PLATFORM_PC )
			{
				StaticMeshDesc.flags |= NX_MF_HARDWARE_MESH;
			}
			MemoryWriteBuffer buf;
			if( !NxCookTriangleMesh(StaticMeshDesc,buf) )
			{
				nLog_Error( _T("Can't create triangle mesh.") );
				return;
			}

			NxTriangleMeshShapeDesc TriangleMeshShapeDesc;
			TriangleMeshShapeDesc.meshData = nx_sdk->createTriangleMesh(MemoryReadBuffer(buf.data));
			StaticMeshActorDesc.shapes.pushBack(&TriangleMeshShapeDesc);
			TriangleMeshShapeDesc.groupsMask		= physx::create_groups_mask(PhysCC_Default, NULL);

			if(  TriangleMeshShapeDesc.meshData == NULL )
			{
				nLog_Error( _T("Can't create triangle mesh.") );
				return;
			}
		}
		else
		{
			NxConvexMeshDesc convexDesc;

			nsimple_mesh* phys_mesh = m_static_mesh->get_simple_mesh();
			if( phys_mesh == NULL ){ return; }

			convexDesc.numVertices			= phys_mesh->vert_buffer.size();
			convexDesc.pointStrideBytes		= sizeof(vector3);
			convexDesc.points				= &(*phys_mesh->vert_buffer.begin());
			convexDesc.flags				= NX_CF_COMPUTE_CONVEX;

			if( !convexDesc.isValid() )
			{	return; }

			// Cooking from memory
			MemoryWriteBuffer buf;
			if( NxCookConvexMesh(convexDesc,buf))
			{
				NxConvexShapeDesc convexShapeDesc;
				convexShapeDesc.meshData = nx_sdk->createConvexMesh(MemoryReadBuffer(buf.data));
				convexShapeDesc.groupsMask = physx::create_groups_mask(PhysCC_Default, NULL);

				if(convexShapeDesc.meshData)
				{
					// Create actor description and instance it.
					StaticMeshActorDesc.shapes.pushBack(&convexShapeDesc);
				}
			}
		}

		NxMat34 static_mesh_comp_tm = physx::nx_matrix_conv(m_mat_world);
		StaticMeshActorDesc.globalPose = static_mesh_comp_tm;
		StaticMeshActorDesc.density = 1.f;
		// If this Actor is not static, make a body for it and flag as kinematic.
		NxBodyDesc BodyDesc;
		if(!m_owner->get_static() )
		{
			if( m_owner->is_kinematic() )
			{
				BodyDesc.flags |= NX_BF_KINEMATIC;
			}
			StaticMeshActorDesc.body = &BodyDesc;
		}

		//// If we are not blocking rigid bodies, disable collision.
		//if(!BlockRigidBody)
		//{
		//	StaticMeshActorDesc.flags = NX_AF_DISABLE_COLLISION;
		//}

		StaticMeshActorDesc.compartment = phys_scene->get_rigidbody_compartment();
		NxActor* nx_static_actor = nx_scene->createActor(StaticMeshActorDesc);
		if( nx_static_actor )
		{
			m_phys_body_instance.reset( nconstruct<nphys_body_instance>( _T("nphys_body_instance") ) );
			m_phys_body_instance->m_nx_actor = nx_static_actor;
			m_phys_body_instance->m_owner_cmp = this;
			m_phys_body_instance->m_owner_actor = this->get_owner();

			nx_static_actor->userData = &m_phys_body_instance;
		}
		else
		{
			nLog_Warning( _T("creat static mesh physics actor error.") );
		}
	}

	void nstatic_mesh_component::on_event(nevent_base& evt)
	{
		if( m_static_mesh
			&& is_resource_ready_event(evt, m_static_mesh->get_name() )
			)
		{
			on_resource_ready();
		}
	}

	void nstatic_mesh_component::on_resource_ready()
	{
		//--注意：必须是可重复调用的
		m_bounds = m_static_mesh->get_bounds();
		m_bounds.transform_by( get_world_matrix() );

		nrender_mesh* mesh = m_static_mesh->get_render_mesh(0);
		
		m_mesh_elements.clear();
		for(size_t i=0; i<mesh->get_num_section(); i++)
		{	
			nrender_mesh_section* sec = mesh->get_section(i);
			nmtl_base* mtl = get_material(0, sec->get_material_id() );
			m_mesh_elements.push_back(mesh_element(this,mesh,sec,mtl,enable_instance()));
		}

		//-- todo：克隆资源材质
	}

	void nstatic_mesh_component::enable_material_clone(bool enable)
	{
		if( !m_clone_mtl
			&& enable) 
		{			
		}

		m_clone_mtl = enable;
	}

	//-----------------------------------------------------------------------------------
	nDEFINE_NAMED_CLASS(nstatic_editor_mesh_component, nstatic_mesh_component)

	const vector<mesh_element>& nstatic_editor_mesh_component::get_mesh_element(int lod)
	{
		if(nengine::instance()->reflection_get_class()->is_kind_of(L"neditor_engine"))
		{
			return nstatic_mesh_component::get_mesh_element(lod);
		}
		else
		{
			return m_empty_meshs;
		}
	}

	nstatic_editor_mesh_component::nstatic_editor_mesh_component( const nstring& name_str ):nstatic_mesh_component(name_str)
	{

	}

	nstatic_editor_mesh_component::~nstatic_editor_mesh_component( void )
	{

	}
}//namespace nexus