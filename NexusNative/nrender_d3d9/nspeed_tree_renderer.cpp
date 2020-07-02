#include "StdAfx.h"
#include "nspeed_tree_renderer.h"
#include "../nengine/framework/view_info.h"
#include "../nengine/speed_tree/nspeed_tree_component.h"
#include "mesh_render.h"

namespace nexus
{
	void spt_instance_adapter::draw_setup_effect(nshading_effect* effect_ptr)
	{
		//-- Upload Wind Matrices
		effect_ptr->set_float("SPT_WindMatrixOffset", tree->wind_matrix_offset);
		effect_ptr->set_matrix_array("SPT_WindMatrices",
			tree->base_spt->get_wind_matrix(),
			c_nNumWindMatricesPerSource);

		//-- alpha mask
		float alpha = 0;
		switch(mesh_type)
		{
		case ESpt_Branch:
			alpha = tree->m_fBranchAlphaValue;
			break;
		case ESpt_Frond:
			alpha = tree->m_fFrondAlphaValue;
			break;
		case ESpt_Leafcard:
		case ESpt_Leafmesh:
			alpha = tree->m_afLeafAlphaValues[0];
			break;
		}
		effect_ptr->set_float("MTL_AlphaMask", alpha/255.0f );

		if( mesh_type == ESpt_Leafcard 
			|| mesh_type == ESpt_Leafmesh )
		{
			//-- camera data
			// query camera angles
			float fCameraAzimuth, fCameraPitch;
			CSpeedTreeRT::GetCameraAngles(fCameraAzimuth, fCameraPitch);

			// adjust azimuth from range [-180,180] to [0,360]
			fCameraAzimuth = nDegToRad(fCameraAzimuth);
			if (fCameraAzimuth < 0.0f)
				fCameraAzimuth += nTwoPI;

			fCameraPitch = nDegToRad(fCameraPitch);
			effect_ptr->set_vector("SPT_CameraAngles", vector2(fCameraAzimuth, fCameraPitch));

			//--
			effect_ptr->set_vector("SPT_LeafAngleScalars", tree->base_spt->get_leafcard_angle_scalars());
			if (tree->base_spt->get_leaf_angle_table() )
			{
				effect_ptr->set_float_array("SPT_LeafAngles",
					tree->base_spt->get_leaf_angle_table() , 4 * c_nNumLeafAnglesPerSource);
			}
		}
	}

	nspeed_tree_renderer::nspeed_tree_renderer(void)
	{
		m_last_time = 0;
	}

	nspeed_tree_renderer::~nspeed_tree_renderer(void)
	{
		clear();
	}

	void nspeed_tree_renderer::clear()
	{
		for(st_tree_map::iterator iter = m_tree_map.begin();
			iter != m_tree_map.end();
			++iter)
		{
			delete iter->second;
		}
		m_tree_map.clear();
		m_last_time = m_timer.elapsed();
	}

	void nspeed_tree_renderer::attach_tree(const nspt_instance* tree)
	{
		nname base_tree_name = tree->base_spt->get_name();
		st_tree_map::iterator iter = m_tree_map.find(base_tree_name);
		if( iter != m_tree_map.end())
		{
			iter->second->trees.push_back(tree);
		}
		else
		{
			tree_group* group = new tree_group;
			group->trees.push_back(tree);
			m_tree_map.insert(make_pair(base_tree_name, group));
		}
	}
	
	void nspeed_tree_renderer::frame_begin(bool bAdvance)
	{
		for(st_tree_map::iterator iter = m_tree_map.begin();
			iter != m_tree_map.end();
			++iter)
		{
			st_tree_array& trees = iter->second->trees;
			nASSERT( !trees.empty() );

			//-- addvance base tree wind
			const nspt_instance* spt0 = trees[0];
			nspeed_base_tree::ptr base_tree = spt0->base_spt;
			if (bAdvance)
			{
				base_tree->advance_speed_wind((float)m_last_time);
			}

			iter->second->inst_adapter.tree = trees[0];

			for (size_t i=0; i<trees.size(); i++)
			{
				const nspt_instance* spt = trees[i];
				iter->second->inst_adapter.instance_datas.push_back(instance_data(vector4(spt->pos.x,spt->pos.y,spt->pos.z,spt->scale),spt->rotation_tri,spt->wind_matrix_offset));
			}

			iter->second->inst_adapter.create_vertex_buffer();
		}
	}

	void nspeed_tree_renderer::draw_all_trees(const nview_info* view, drawing_policy* dp, bool enable_instance)
	{
		if( enable_instance )
			instance_draw(view, dp);
		else
			draw_no_instance(view, dp);
	}

	class nspt_render_mesh_adapter : public nrender_mesh
	{
		const nspt_instance* m_spt;
		nrender_static_mesh_indexed* m_mesh;
		ESptMeshType m_mesh_type;
		int	m_leaf_index;
	public:
		nspt_render_mesh_adapter(const nspt_instance* spt, nrender_static_mesh_indexed* mesh, ESptMeshType mesh_type)
			: m_spt(spt),m_mesh(mesh),m_mesh_type(mesh_type),m_leaf_index(0)
		{}
		void set_leaf_index(int i)
		{
			m_leaf_index = i;
		}
		virtual nvertex_factory* get_vertex_factory()
		{
			return m_mesh->get_vertex_factory();
		}

		virtual size_t get_num_section()
		{
			return m_mesh->get_num_section();
		}

		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			return m_mesh->get_section(section_index);
		}
		
		virtual void draw_setup_effect(nshading_effect* effect_ptr)	
		{
			//-- Upload Wind Matrices
			effect_ptr->set_float("SPT_WindMatrixOffset", m_spt->wind_matrix_offset);
			effect_ptr->set_matrix_array("SPT_WindMatrices",
				m_spt->base_spt->get_wind_matrix(),
				c_nNumWindMatricesPerSource);

			//-- setup speed tree instance mesh param
			vector4 pos(m_spt->pos.x,m_spt->pos.y,m_spt->pos.z, m_spt->scale);
			effect_ptr->set_vector("SPT_TreePos", pos);
			effect_ptr->set_vector("SPT_TreeRot", m_spt->rotation_tri);

			//-- alpha mask
			float alpha = 0;
			switch(m_mesh_type)
			{
			case ESpt_Branch:
				alpha = m_spt->m_fBranchAlphaValue;
				break;
			case ESpt_Frond:
				alpha = m_spt->m_fFrondAlphaValue;
				break;
			case ESpt_Leafcard:
			case ESpt_Leafmesh:
				alpha = m_spt->m_afLeafAlphaValues[0];
				break;
			}
			effect_ptr->set_float("MTL_AlphaMask", alpha/255.0f );

			if( m_mesh_type == ESpt_Leafcard 
				|| m_mesh_type == ESpt_Leafmesh )
			{
				//-- camera data
				// query camera angles
				float fCameraAzimuth, fCameraPitch;
				CSpeedTreeRT::GetCameraAngles(fCameraAzimuth, fCameraPitch);

				// adjust azimuth from range [-180,180] to [0,360]
				fCameraAzimuth = nDegToRad(fCameraAzimuth);
				if (fCameraAzimuth < 0.0f)
					fCameraAzimuth += nTwoPI;

				fCameraPitch = nDegToRad(fCameraPitch);
				effect_ptr->set_vector("SPT_CameraAngles", vector2(fCameraAzimuth, fCameraPitch));

				//--
				effect_ptr->set_vector("SPT_LeafAngleScalars", m_spt->base_spt->get_leafcard_angle_scalars());
				if (m_spt->base_spt->get_leaf_angle_table() )
				{
					effect_ptr->set_float_array("SPT_LeafAngles",
						m_spt->base_spt->get_leaf_angle_table() , 4 * c_nNumLeafAnglesPerSource);
				}
			}
		}
	};

	void nspeed_tree_renderer::draw_no_instance(const nview_info* view, drawing_policy* dp)
	{
		const matrix44& vm = view->mat_view;

		vector3 camera_pos = view->eye_pos;
		vector3 camera_dir = vector3(vm._13, vm._23, vm._33);
		CSpeedTreeRT::SetCamera((float*)&camera_pos, (float*)&camera_dir);

		//--
		for(st_tree_map::iterator iter = m_tree_map.begin();
			iter != m_tree_map.end();
			++iter)
		{
			st_tree_array& trees = iter->second->trees;
			nASSERT( !trees.empty() );

			//-- advance base tree wind
			const nspt_instance* spt0 = trees[0];
			nspeed_base_tree::ptr base_tree = spt0->base_spt;
			
			//-- draw branches -----
			if( base_tree->has_branch() )
			{
				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];

					nrender_static_mesh_indexed* branch_mesh = spt->base_spt->get_branches_mesh();
					nspt_render_mesh_adapter adapter(spt, branch_mesh, ESpt_Branch);
					for( size_t j=0; j<branch_mesh->get_num_section(); j++ )
					{
						dp->draw(view,mesh_element(spt->owner, &adapter, branch_mesh->get_section(j),
							const_cast<nmtl_base*>(base_tree->get_branches_material()),false));
					}
				}
			}// end of if

			//-- draw fronds ----
			if( base_tree->has_frond() )
			{
				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];

					nrender_static_mesh_indexed* frond_mesh = spt->base_spt->get_frond_mesh();
					nspt_render_mesh_adapter adapter(spt, frond_mesh, ESpt_Frond);
					for( size_t j=0; j<frond_mesh->get_num_section(); j++ )
					{
						dp->draw(view,mesh_element(spt->owner, &adapter, frond_mesh->get_section(j),
							const_cast<nmtl_base*>(base_tree->get_frond_material()),false));
					}
				}
			}

			//-- draw leafcards
			if( base_tree->has_leafcard())
			{
				nrender_static_mesh_indexed* leafcard = base_tree->get_leafcard_lod_mesh(0);
				nmtl_base* mtl = const_cast<nmtl_base*>(base_tree->get_leaf_material());
				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];
					int lod = spt->m_anLeafLodLevels[0];
					
					nspt_render_mesh_adapter adapter(spt, leafcard, ESpt_Leafcard);		
					adapter.set_leaf_index(0);

					for( size_t j=0; j<leafcard->get_num_section(); j++ )
					{
						dp->draw(view,mesh_element(spt->owner, &adapter, leafcard->get_section(j),
							mtl,false));
					}
				}
			}

			if (base_tree->has_leafmesh() )
			{
				nrender_static_mesh_indexed* leafmesh = base_tree->get_leafmesh();
				nmtl_base* mtl = const_cast<nmtl_base*>(base_tree->get_leaf_material());

				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];
					nspt_render_mesh_adapter adapter(spt, leafmesh, ESpt_Leafmesh);
					adapter.set_leaf_index(0);
		
					for( size_t j=0; j<leafmesh->get_num_section(); j++ )
					{
						dp->draw(view,mesh_element(spt->owner, &adapter, leafmesh->get_section(j),mtl,false));
					}
				}
			}

		}// end of for
	}

	void nspeed_tree_renderer::instance_draw(const nview_info* view, drawing_policy* dp)
	{
		const matrix44& vm = view->mat_view;
		
		vector3 camera_pos = view->eye_pos;
		vector3 camera_dir = vector3(vm._13, vm._23, vm._33);
		CSpeedTreeRT::SetCamera((float*)&camera_pos, (float*)&camera_dir);

		//--
		for(st_tree_map::iterator iter = m_tree_map.begin();
			iter != m_tree_map.end();
			++iter)
		{
			st_tree_array& trees = iter->second->trees;
			nASSERT( !trees.empty() );

			//-- advance base tree wind
			const nspt_instance* spt0 = trees[0];
			nspeed_base_tree::ptr base_tree = spt0->base_spt;
			spt_instance_adapter& inst_adapter = iter->second->inst_adapter;

			//-- draw branches -----
			if( base_tree->has_branch() )
			{
				nrender_static_mesh_indexed* branch_mesh = base_tree->get_branches_mesh();
				inst_adapter.m_mesh = branch_mesh;
				inst_adapter.create_instance_vertex_factory( static_cast<d3d9_vertex_factory*>(branch_mesh->get_vertex_factory()));
				inst_adapter.mesh_type = ESpt_Branch;
			
				dp->draw( view,mesh_element(NULL,&inst_adapter,branch_mesh->get_section(0),const_cast<nmtl_base*>(base_tree->get_branches_material()),false));
			}

			//-- draw fronds ----
			if( base_tree->has_frond() )
			{
				nrender_static_mesh_indexed*  mesh = base_tree->get_frond_mesh();
				inst_adapter.m_mesh = mesh;

				inst_adapter.create_instance_vertex_factory( static_cast<d3d9_vertex_factory*>(mesh->get_vertex_factory()));
				inst_adapter.mesh_type = ESpt_Frond;
				
				dp->draw(view,mesh_element(NULL,&inst_adapter,mesh->get_section(0),
					const_cast<nmtl_base*>(base_tree->get_frond_material()),false));
			}

			//-- draw leafcards
			if (base_tree->has_leafcard())
			{	
				nrender_mesh * mesh = base_tree->get_leafcard_lod_mesh(0);
				inst_adapter.m_mesh = mesh;
				inst_adapter.create_instance_vertex_factory( static_cast<d3d9_vertex_factory*>(mesh->get_vertex_factory()));
				inst_adapter.mesh_type = ESpt_Leafcard;
			
				dp->draw(view,mesh_element(NULL,&inst_adapter,mesh->get_section(0),
					const_cast<nmtl_base*>(base_tree->get_leaf_material()),false));
			}

			if (base_tree->has_leafmesh())
			{	
				nrender_mesh * mesh = base_tree->get_leafmesh();
				inst_adapter.m_mesh = mesh;
				inst_adapter.create_instance_vertex_factory( static_cast<d3d9_vertex_factory*>(mesh->get_vertex_factory()));
				inst_adapter.mesh_type = ESpt_Leafmesh;
		
				dp->draw(view,mesh_element(NULL,&inst_adapter,mesh->get_section(0),const_cast<nmtl_base*>(base_tree->get_leaf_material()),false));
			}
		}// end of for
	}
}//namespace nexus