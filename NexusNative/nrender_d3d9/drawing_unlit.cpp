#include "StdAfx.h"
#include "drawing_unlit.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"

namespace nexus
{
	nstring	drawing_unlit::s_dp_name_str(_T("drawing_unlit"));

	drawing_unlit::drawing_unlit(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
		m_policy_tech = EDP_Unlit;
	}

	drawing_unlit::~drawing_unlit(void)
	{
	}

	void drawing_unlit::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}//namespace nexus