#include "StdAfx.h"
#include "drawing_base_pass.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"

namespace nexus
{
	nstring	drawing_base_pass::s_dp_name_str(_T("drawing_base_pass"));

	drawing_base_pass::drawing_base_pass(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
	}

	drawing_base_pass::~drawing_base_pass(void)
	{
	}

	void drawing_base_pass::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
		if(res_ptr == NULL)
			return;

		int lod = obj->get_render_lod();
		for(size_t i=0; i<res_ptr->get_num_section(); i++)
		{
			enum ETransparentType trans = obj->get_transparent_type((int)lod, i);
			// 透明物体需要最后画出，所有bass pass不处理它们
			if( trans == ETrans_UnlitTrans)
				continue;

			draw_shared_material(view, obj, i);
		}
	}

	void drawing_base_pass::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}//namespace nexus