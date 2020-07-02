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
#include "draw_hit_proxy.h"
#include "shadow_buffer.h"

namespace nexus
{
	void drawing_policy_type::load_shader_file()
	{
		//-- load shader code from name/file
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

	template<typename TLight, typename TShadow>
	void create_light_type(dp_type_list& dl)
	{
		drawing_light<TLight, TShadow, true, true>::create_type(dl);
		drawing_light<TLight, TShadow, true, false>::create_type(dl);
		drawing_light<TLight, TShadow, false, true>::create_type(dl);
		drawing_light<TLight, TShadow, false, false>::create_type(dl);
	}

	void dp_type_list::create_all_type()
	{
		clear();
		//--
		drawing_pre_pass::create_type(*this);
		drawing_wireframe::create_type(*this);
		drawing_unlit::create_type(*this);
		draw_hit_proxy::create_type(*this);
		
		drawing_base_pass::create_type(*this);
		drawing_unlit_trans::create_type(*this);

		create_light_type<point_light_policy, noshadow>(*this);
		create_light_type<directional_light_policy, noshadow>(*this);

		create_light_type<point_light_policy, shadow_buffer>(*this);
		create_light_type<directional_light_policy, shadow_buffer>(*this);
	}
}//namespace nexus