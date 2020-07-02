#include "StdAfx.h"
#include "nskeletal_mesh_component.h"
#include "../framework/nengine.h"
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

		//-- create render mesh
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
		{}

		//-- todo : get bounds
		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
	}

	nskel_anim_controller::ptr nskeletal_mesh_component::reset_anim_controller(const nstring& anim_ctrl_class_name)
	{
		m_anim_ctrl.reset( nconstruct<nskel_anim_controller>(anim_ctrl_class_name) );
		return m_anim_ctrl;
	}

	void nskeletal_mesh_component::update(float delta_time, const view_info& view)
	{
		(void)view;

		if( m_anim_ctrl )
		{
			m_anim_ctrl->advance_anim(delta_time);
		}
	}

	void nskeletal_mesh_component::render(const view_info& view)
	{
		if( m_mesh_res
			&& m_mesh_res->ready()
			&& m_anim_ctrl )
		{
			if( m_cpu_skin )
			{
				size_t cur_lod = 0;	// todo: 
				m_cpu_skin_meshes[cur_lod]->update_skin(m_anim_ctrl->get_matrix_palette());
			}

			nmesh_component::render(view);
		}
	}

	nrender_mesh* nskeletal_mesh_component::get_render_mesh(int lod)
	{
		if(m_cpu_skin)
		{
			return m_cpu_skin_meshes[lod];
		}
		else
		{
			return NULL;
		}
	}

	nmaterial_base* nskeletal_mesh_component::get_material(int lod, int mtl_id)
	{
		nmaterial_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		return m_mesh_res->get_material(lod, mtl_id);
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
		for(size_t i=1; i<num_bone; i++)
		{
			const bone_info& this_bone = skel->m_bones[i];
			
			vector3 start_pt(0,0,0), end_pt(0,0,0);
			matrix44 bone_mat = ref_base[i]*mat_pal[i];
			start_pt = bone_mat.get_origin();

			matrix44 parent_mat = ref_base[this_bone.parent_index]*mat_pal[this_bone.parent_index];
			end_pt = parent_mat.get_origin();

			PDI->draw_line(start_pt, end_pt, color4f(1, 1, 0, 1));

			const float axis_len = 4;
			end_pt = bone_mat.get_axis_x()*axis_len;
			PDI->draw_line(start_pt, end_pt+start_pt, color4f(1, 0, 0, 1));

			end_pt = bone_mat.get_axis_y()*axis_len;
			PDI->draw_line(start_pt, end_pt+start_pt, color4f(0, 1, 0, 1));

			end_pt = bone_mat.get_axis_z()*axis_len;
			PDI->draw_line(start_pt, end_pt+start_pt, color4f(0, 0, 1, 1));
		}
		PDI->end_line_batch();
	}
}//namespace nexus