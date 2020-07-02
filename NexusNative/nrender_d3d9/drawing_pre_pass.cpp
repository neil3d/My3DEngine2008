#include "StdAfx.h"
#include "drawing_pre_pass.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"
#include "d3d_device_manager.h"
#include "d3d9_mesh_section.h"
#include "nrenderer_d3d9.h"
#include "util.h"

namespace nexus
{	
	nstring	drawing_pre_pass::s_dp_name_str(_T("drawing_pre_pass"));

	drawing_pre_pass::drawing_pre_pass(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
		m_policy_tech = EDP_PreDepth;
	}

	drawing_pre_pass::~drawing_pre_pass(void)
	{
	}

	void drawing_pre_pass::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}
}//namespace nexus