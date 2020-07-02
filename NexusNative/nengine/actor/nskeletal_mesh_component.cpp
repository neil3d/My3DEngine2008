#include "StdAfx.h"
#include "nskeletal_mesh_component.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../renderer/nrender_resource_manager.h"
#include "../framework/world_define.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nskeletal_mesh_component, nmesh_component)

	nskeletal_mesh_component::nskeletal_mesh_component(const nstring& name_str):nmesh_component(name_str)
	{
		m_cpu_skin = false;
	}

	nskeletal_mesh_component::~nskeletal_mesh_component(void)
	{
	}

	void nskeletal_mesh_component::reset_resource(nresource_skeletal_mesh::ptr res_mesh, bool cpu_skin)
	{
		m_mesh_res = res_mesh;
		m_cpu_skin = cpu_skin;

		if( m_mesh_res
			&& m_mesh_res->ready() )
			on_resource_ready();
	}

	void nskeletal_mesh_component::reset_resource( const resource_location& loc, bool cpu_skin )
	{
		reset_resource(nresource_manager::instance()->load_skeletal_mesh(loc), cpu_skin);
	}

	nskel_anim_controller::ptr nskeletal_mesh_component::reset_anim_controller(const nstring& anim_ctrl_class_name)
	{
		m_anim_ctrl.reset( nconstruct<nskel_anim_controller>(anim_ctrl_class_name) );
		return m_anim_ctrl;
	}

	void nskeletal_mesh_component::update(float delta_time, const nviewport& view)
	{
		nmesh_component::update(delta_time, view);

		if( !m_mesh_res
			|| !m_mesh_res->ready())
			return;

		if( m_anim_ctrl )
		{
			m_anim_ctrl->tick_anim(delta_time);
		}
	}

	void nskeletal_mesh_component::render(render_package_base* rnd)
	{
		if( m_mesh_res
			&& m_mesh_res->ready() )
		{
			if( m_cpu_skin && !m_cpu_skin_meshes.empty() )
			{
				if(m_anim_ctrl)
				{
					size_t cur_lod = 0;	// todo: 
					m_cpu_skin_meshes[cur_lod]->update_skin(m_anim_ctrl->get_matrix_palette());
				}

				nmesh_component::render(rnd);
			}			
		}
	}

	nmtl_base*  nskeletal_mesh_component::get_material(int lod, int mtl_id)
	{
		nmtl_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		nmtl_base::ptr res_mtl;
		if( m_mesh_res
			&& m_mesh_res->ready() )
		{
			res_mtl = m_mesh_res->get_material(lod, mtl_id);			
		}
		if( res_mtl)
			return res_mtl.get();
		else		
			return nresource_manager::instance()->get_default_material();
	}

	void nskeletal_mesh_component::draw_skeleton(nrender_primitive_draw_interface* PDI)
	{
		if( !m_anim_ctrl)
			return;
		
		const nskeleton_define* skel = m_anim_ctrl->get_skeleton();
		if( skel == NULL )
			return;


		PDI->begin_line_batch();
		const matrix_palette& mat_pal = m_anim_ctrl->get_matrix_palette();
		const matrix_palette& ref_base = skel->m_ref_base;

		size_t num_bone = skel->m_bones.size();
		vector3 box_size(1,1,1);
		for(size_t i=1; i<num_bone; i++)
		{
			const bone_info& this_bone = skel->m_bones[i];
			
			vector3 start_pt(0,0,0), end_pt(0,0,0);
			matrix44 bone_mat = ref_base[i]*mat_pal[i]*get_world_matrix();
			start_pt = start_pt*bone_mat;

			matrix44 parent_mat = ref_base[this_bone.parent_index]*mat_pal[this_bone.parent_index]*get_world_matrix();
			end_pt = end_pt*parent_mat;

			PDI->draw_line(start_pt, end_pt, color4f(1, 1, 0, 1));			
			PDI->draw_wired_box(AABBox(start_pt-box_size, start_pt+box_size), color4f(1,0,0,1));
		}
		PDI->end_line_batch();
	}

	void nskeletal_mesh_component::_destroy()
	{
		m_mesh_res.reset();
		m_anim_ctrl.reset();
		m_cpu_skin_meshes.clear();

		nmesh_component::_destroy();
	}

	void nskeletal_mesh_component::_on_device_lost(int param)
	{
		(void)param;
		m_cpu_skin_meshes.clear();
	}

	bool nskeletal_mesh_component::_on_device_reset(int param)
	{
		(void)param;
		if( m_mesh_res && m_anim_ctrl)
			on_resource_ready();
		return true;
	}

	void nskeletal_mesh_component::create_render_mesh()
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		if(m_cpu_skin)
		{
			for(size_t i=0; i< m_mesh_res->get_num_lod(); i++)
			{
				nrender_cpu_skin_mesh* skin_mesh = rres_mgr->alloc_cup_skin_mesh();
				skin_mesh->create( m_mesh_res->get_lod_mesh(i) );

				m_cpu_skin_meshes.push_back( skin_mesh );					
			}
		}
		else
		{
			nLog_Error(_T("Only support cpu skin!"));
		}
	}

	void nskeletal_mesh_component::on_event(nevent_base& evt)
	{
		if( m_mesh_res
			&& is_resource_ready_event(evt, m_mesh_res->get_name()) )
		{
			on_resource_ready();
		}
	}

	void nskeletal_mesh_component::on_resource_ready()
	{
		//-- 注意：必须是可重复调用的

		//-- create render mesh
		create_render_mesh();

		//-- get bounds
		if(m_mesh_res && m_mesh_res->get_num_lod() > 0)
		{
			m_mesh_res->get_bounds(m_bounds);
			m_bounds.transform_by( get_world_matrix() );
		}

		if(m_cpu_skin)
		{
			nrender_mesh* mesh = m_cpu_skin_meshes[0];
			m_mesh_elements.clear();
			for(size_t i=0; i<mesh->get_num_section(); i++)
			{	
				nrender_mesh_section* sec = mesh->get_section(i);
				nmtl_base* mtl = get_material(0, sec->get_material_id() );
				m_mesh_elements.push_back(mesh_element(this,mesh,sec,mtl));
			}
		}
		else
		{
			nLog_Error(_T("Only support cpu skin!"));
		}
	}

	void nskeletal_mesh_component::_update_transform(const object_space& parent_space)
	{
		nmesh_component::_update_transform(parent_space);

		if( m_mesh_res
			&& m_mesh_res->ready() )
		{
			m_mesh_res->get_bounds(m_bounds);
			m_bounds.transform_by( get_world_matrix() );
		}

		nscene_event evt(ESA_MovePrimitive, this);
		m_owner->on_event(evt);
	}
}//namespace nexus