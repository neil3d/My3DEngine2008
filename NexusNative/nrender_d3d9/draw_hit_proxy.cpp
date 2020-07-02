#include "StdAfx.h"
#include "draw_hit_proxy.h"
#include "shading_effect.h"
#include "d3d9_vertex_factory.h"

namespace nexus
{
	nstring	draw_hit_proxy::s_dp_name_str(_T("draw_hit_proxy"));

	draw_hit_proxy::draw_hit_proxy(void):m_tech("techDefault")
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
	}

	draw_hit_proxy::~draw_hit_proxy(void)
	{
	}

	void draw_hit_proxy::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}

	void draw_hit_proxy::setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index)
	{
		drawing_policy::setup_effect(effect_ptr, obj, section_index);

		HRESULT hr = effect_ptr->get_d3d_effect()->SetTechnique(m_tech.c_str());

		hit_id hid = obj->get_hit_proxy_id();
		color4ub c;
		memcpy(&c, &hid, sizeof(hid));
		vector4 v(c.B/255.0f, c.G/255.0f, c.R/255.0f, c.A/255.0f);
		effect_ptr->set_vector("Sys_HitProxyID", v);
	}

	void draw_hit_proxy::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		int lod = obj->get_render_lod();
		nrender_mesh* res_ptr = obj->get_render_mesh(lod);
		if(res_ptr == NULL)
			return;

		// 找到无需material的shader
		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>(res_ptr->get_vertex_factory());		

		for(size_t i=0; i<res_ptr->get_num_section(); i++)
		{
			enum ETransparentType trans = obj->get_transparent_type(lod, i);

			// 透明物体无法处理
			if( trans == ETrans_UnlitTrans)
				continue;


			nrender_mesh_section* sec = res_ptr->get_section(i);
			nmaterial_base* mtl = obj->get_material( obj->get_render_lod(), sec->get_material_id() );
			bool two_sided = false;
			if( mtl )
				two_sided = mtl->is_two_sided();

			d3d9_shading_effect* shader = shading_effect_lib::instance()->acquire_shader_default_mtl(
				m_type.get(), vf->get_type(), 
				res_ptr->get_shader_mod(),
				two_sided );

			draw_shared_no_material(shader, view, obj, i);			
		}// end of for
	}
}//namespace nexus