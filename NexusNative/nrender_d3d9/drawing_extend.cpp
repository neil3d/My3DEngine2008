#include "StdAfx.h"
#include "drawing_extend.h"
#include "shading_effect.h"

namespace nexus
{
	nstring	drawing_extend::s_dp_name_str(_T("drawing_extend"));

	drawing_extend::drawing_extend(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
		m_policy_tech = EDP_Num;
	}

	drawing_extend::~drawing_extend(void)
	{
	}

	void drawing_extend::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}