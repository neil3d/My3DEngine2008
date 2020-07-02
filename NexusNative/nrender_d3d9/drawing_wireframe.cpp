#include "StdAfx.h"
#include "drawing_wireframe.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"

namespace nexus
{
	nstring	drawing_wireframe::s_dp_name_str(_T("drawing_wireframe"));

	drawing_wireframe::drawing_wireframe(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
	}

	drawing_wireframe::~drawing_wireframe(void)
	{
	}

	void drawing_wireframe::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
		if(res_ptr == NULL)
			return;

		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>(res_ptr->get_vertex_factory());
		d3d9_shading_effect* shader = shading_effect_lib::instance()->acquire_shader(m_type.get(), vf->get_type(),
			NULL, res_ptr->get_shader_mod());

		for(size_t i=0; i<res_ptr->get_num_section(); i++)
			draw_shared_no_material(shader, view, obj, i);
	}

	void drawing_wireframe::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}//namespace nexus