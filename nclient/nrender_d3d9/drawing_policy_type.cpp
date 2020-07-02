#include "StdAfx.h"
#include <sstream>
#include "util.h"
#include "drawing_policy_type.h"
#include "drawing_pre_pass.h"
#include "drawing_wireframe.h"
#include "drawing_unlit.h"
#include "drawing_base_pass.h"
#include "drawing_unlit_trans.h"
#include "drawing_light.h"

namespace nexus
{
	void drawing_policy_type::load_shader_code_from_name()
	{
		wostringstream ss;
		ss << _T("shader_d3d9/")
			<< dp_name.name_str
			<< _T(".fx");

		load_shder_source(ss.str(), shader_source);
	}

	dp_type_list::dp_type_list(void)
	{}

	dp_type_list::~dp_type_list(void)
	{}

	void dp_type_list::add_drawing_policy_type(drawing_policy_type::ptr dp_ptr)
	{
		m_dp_type_map.insert( make_pair(dp_ptr->dp_name, dp_ptr) );
	}

	drawing_policy_type::ptr dp_type_list::find_drawing_policy_type(const nstring& dp_name_str)
	{
		nname nm(dp_name_str);

		dp_type_map::iterator iter = m_dp_type_map.find(nm);
		if( iter == m_dp_type_map.end() )
			return drawing_policy_type::ptr();
		else
			return iter->second;
	}

	void dp_type_list::clear()
	{
		m_dp_type_map.clear();
	}

	void dp_type_list::create_all_type()
	{
		clear();
		//--
		drawing_pre_pass::create_type(*this);
		drawing_wireframe::create_type(*this);
		drawing_unlit::create_type(*this);
		
		drawing_base_pass::create_type(*this);
		drawing_unlit_trans::create_type(*this);

		drawing_light<point_light_policy>::create_type(*this);
		drawing_light<directional_light_policy>::create_type(*this);
	}
}//namespace nexus