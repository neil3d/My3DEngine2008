#include "StdAfx.h"
#include "drawing_unlit_trans.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"

namespace nexus
{
	nstring	drawing_unlit_trans::s_dp_name_str(_T("drawing_unlit_trans"));

	drawing_unlit_trans::drawing_unlit_trans(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
	}

	drawing_unlit_trans::~drawing_unlit_trans(void)
	{
	}

	void drawing_unlit_trans::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
		if(res_ptr == NULL)
			return;

		for(size_t i=0; i<res_ptr->get_num_section(); i++)
		{
			draw_shared_material(view, obj, i);
		}
	}

	void drawing_unlit_trans::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}//namespace nexus