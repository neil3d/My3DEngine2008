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
		m_policy_tech = EDP_Wireframe;
	}

	drawing_wireframe::~drawing_wireframe(void)
	{
	}

	void drawing_wireframe::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}

	void drawing_wireframe::setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* mesh_comp)
	{
		(void)mesh_comp;
		effect_ptr->set_render_state(ERS_CullMode,ECM_None);
	}
}//namespace nexus