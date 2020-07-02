#include "StdAfx.h"
#include "nspeed_tree_renderer.h"

namespace nexus
{
	void nspt_render_proxy::draw_setup_effect(nshading_effect* effect_ptr) const
	{
		//-- setup speed tree instance mesh param
		vector4 pos(tree->pos.x,tree->pos.y,tree->pos.z, tree->scale);
		effect_ptr->set_vector("SPT_TreePos", pos);
		effect_ptr->set_vector("SPT_TreeRot", tree->rotation_tri);

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
			alpha = tree->m_afLeafAlphaValues[leaf_index];
			break;
		}
		effect_ptr->set_int("MTL_AlphaMask", int(alpha) );

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
			effect_ptr->set_float_array("SPT_LeafAngles",
				tree->base_spt->get_leaf_angle_table(), 4 * c_nNumLeafAnglesPerSource);
		}
	}

	nspeed_tree_renderer::nspeed_tree_renderer(void)
	{
		m_last_time = 0;
	}

	nspeed_tree_renderer::~nspeed_tree_renderer(void)
	{
	}

	void nspeed_tree_renderer::clear()
	{
		m_tree_map.clear();
		m_last_time = m_timer.elapsed();
	}

	void nspeed_tree_renderer::attach_tree(const nspt_instance* tree)
	{
		nname base_tree_name = tree->base_spt->get_name();
		st_tree_map::iterator iter = m_tree_map.find(base_tree_name);
		if( iter != m_tree_map.end())
		{
			iter->second.push_back(tree);
		}
		else
		{
			st_tree_array new_array;
			new_array.push_back(tree);
			m_tree_map.insert(make_pair(base_tree_name, new_array));
		}
	}

	void nspeed_tree_renderer::draw_all_trees(const d3d_view_info* view, drawing_policy* dp)
	{
		vector3 camera_pos = view->eye_pos;
		vector3 camera_dir = view->mat_view.get_axis_z();
		CSpeedTreeRT::SetCamera((float*)&camera_pos, (float*)&camera_dir);

		//--
		for(st_tree_map::iterator iter = m_tree_map.begin();
			iter != m_tree_map.end();
			++iter)
		{
			st_tree_array& trees = iter->second;
			nASSERT( !trees.empty() );

			//-- addvance base tree wind
			const nspt_instance* spt0 = trees[0];
			nspeed_base_tree::ptr base_tree = spt0->base_spt;
			base_tree->advance_speed_wind(m_last_time);

			//-- draw branches -----
			if( base_tree->has_branch() )
			{
				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];

					nrender_static_mesh_indexed* branch_mesh = spt->base_spt->get_branches_mesh();
					if( branch_mesh->get_num_section() > spt->m_nBranchLodLevel )
					{
						m_render_obj.tree = spt;
						m_render_obj.mesh_type = ESpt_Branch;
						m_render_obj.mesh = NULL;
						m_render_obj.mesh_adapter.m_lod = spt->m_nBranchLodLevel;
						m_render_obj.mesh_adapter.m_mesh = spt->base_spt->get_branches_mesh();
						m_render_obj.mtl = const_cast<nmaterial_base*>(spt->base_spt->get_branches_material());						

						dp->draw_mesh(view, &m_render_obj);
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
					if( frond_mesh->get_num_section() > spt->m_nFrondLodLevel )
					{
						m_render_obj.tree = spt;
						m_render_obj.mesh_type = ESpt_Frond;
						m_render_obj.mesh = NULL;
						m_render_obj.mesh_adapter.m_lod = spt->m_nFrondLodLevel;
						m_render_obj.mesh_adapter.m_mesh = frond_mesh;
						m_render_obj.mtl = const_cast<nmaterial_base*>(spt->base_spt->get_frond_material());

						dp->draw_mesh(view, &m_render_obj);
					}
				}
			}

			//-- draw leafcards
			if( base_tree->has_leafcard() 
				|| base_tree->has_leafmesh() )
			{
				for (size_t i=0; i<trees.size(); i++)
				{
					const nspt_instance* spt = trees[i];
					draw_leaf(0, spt, view, dp);
					draw_leaf(1, spt, view, dp);
				}
			}

		}// end of for
	}

	void nspeed_tree_renderer::draw_leaf(int index, const nspt_instance* spt, const d3d_view_info* view, drawing_policy* dp)
	{
		int lod = spt->m_anLeafLodLevels[index];
		if( lod > -1 )
		{
			m_render_obj.tree = spt;
			m_render_obj.mesh_type = ESpt_Leafcard;
			m_render_obj.mesh = spt->base_spt->get_leafcard_lod_mesh(lod);
			m_render_obj.mtl = const_cast<nmaterial_base*>(spt->base_spt->get_leaf_material());
			m_render_obj.leaf_index = index;

			if( m_render_obj.mesh )
				dp->draw_mesh(view, &m_render_obj);

			nrender_static_mesh_indexed* leafmesh = spt->base_spt->get_leafmesh();
			if( leafmesh
				&& leafmesh->get_num_section() > lod )
			{
				m_render_obj.mesh_type = ESpt_Leafmesh;
				m_render_obj.mesh = NULL;
				m_render_obj.mesh_adapter.m_lod = lod;
				m_render_obj.mesh_adapter.m_mesh = spt->base_spt->get_leafmesh();

				dp->draw_mesh(view, &m_render_obj);
			}
		}// end of if
	}
}//namespace nexus