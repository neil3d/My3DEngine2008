#include "StdAfx.h"
#include "drawing_policy.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"
#include "d3d_device_manager.h"
#include "d3d9_mesh_section.h"
#include "nrenderer_d3d9.h"
#include "util.h"


namespace nexus
{
	drawing_policy::drawing_policy(void)
	{
	}

	void drawing_policy::draw_shared_material(const d3d_view_info* view, const nrender_proxy* obj, size_t section_index)
	{
		nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
		if(res_ptr == NULL)
			return;

		d3d9_mesh_section* mesh_sec = static_cast<d3d9_mesh_section*>(res_ptr->get_section(section_index));		
		
		nmaterial_base* mtl = obj->get_material(obj->get_render_lod(), mesh_sec->get_material_id());
				
		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>(res_ptr->get_vertex_factory());		
		nASSERT(vf != NULL);
		if(vf == NULL)
			return;

		d3d9_shading_effect* shader = shading_effect_lib::instance()->acquire_shader(m_type.get(), vf->get_type(), 
			mtl, res_ptr->get_shader_mod());
		if(shader == NULL
			|| shader->get_d3d_effect()==NULL)
		{
			nTRACE(_T("shader effect not find.\r\n"));
			return;
		}

		vf->draw_set_full();

		shader->set_system_parameter(view, obj);
		if( mtl )
			mtl->draw_effect_param(shader);		
		setup_effect(shader, obj, section_index);
		res_ptr->draw_setup_effect(shader);
		obj->draw_setup_effect(shader);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXEffect* d3d_effect = shader->get_d3d_effect();
		UINT num_pass = 0;		

		mesh_sec->draw_set_index_buffer();

		HRESULT hr = d3d_effect->Begin(&num_pass, 0);
		if( SUCCEEDED(hr) )
		{
			for(UINT i=0; i<num_pass; i++)
			{
				hr = d3d_effect->BeginPass(i);
				mesh_sec->draw_call(vf->get_num_vert());				
				d3d_effect->EndPass();
			}
		}
		hr = d3d_effect->End();
	}

	void drawing_policy::draw_shared_no_material(d3d9_shading_effect* shader, const d3d_view_info* view, const nrender_proxy* obj,size_t section_index)
	{
		if(shader == NULL
			|| shader->get_d3d_effect()==NULL)
		{
			nTRACE(_T("nexus error: shader not found\r\n"));
			return;
		}

		nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
		if(res_ptr == NULL)
			return;

		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>(res_ptr->get_vertex_factory());

		if(vf == NULL)
			return;

		vf->draw_set_pos_only();

		shader->set_system_parameter(view, obj);
		setup_effect(shader, obj, section_index);
		res_ptr->draw_setup_effect(shader);
		obj->draw_setup_effect(shader);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXEffect* d3d_effect = shader->get_d3d_effect();
		UINT num_pass = 0;

		HRESULT hr = d3d_effect->Begin(&num_pass, 0);
		if( SUCCEEDED(hr) )
		{
			for(UINT i=0; i<num_pass; i++)
			{
				hr = d3d_effect->BeginPass(i);

				d3d9_mesh_section* mesh_sec = static_cast<d3d9_mesh_section*>(res_ptr->get_section(section_index));
				mesh_sec->draw_set_index_buffer();
				mesh_sec->draw_call(vf->get_num_vert());
				
				d3d_effect->EndPass();
			}
		}
		hr = d3d_effect->End();
	}

	void drawing_policy::default_create_type(dp_type_list& type_list, const nstring& dp_name)
	{
		drawing_policy_type::ptr new_type(
			nNew drawing_policy_type(dp_name)
			);
		new_type->load_shader_file();

		type_list.add_drawing_policy_type(new_type);
	}

	void drawing_policy::setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index)
	{}
}//namespace nexus