/**
*	Nexus Engine
*
*	Author: mr
*	Date:	Nov, 2010
*/

#include "base_pass_policy_type.h"
#pragma once

namespace nexus
{
	/**
	*	环境光策略
	*/
	class ambent_light_policy
	{
	public:
		struct context 
		{
		};

		ambent_light_policy(void);
		virtual ~ambent_light_policy(void);

		static nstring get_name(void)	{	return nstring(_T("Ambent_none"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type)
		{
			dp_type->macros.push_back(shader_define("NO_AMBENT_LIGHT", ""));
		}

		static void set_effect_parameter( d3d9_shading_effect* effect_ptr,const context& data)
		{
			(void)effect_ptr;
			(void)data;
		}
	};

	class hemisphere_policy :public ambent_light_policy
	{
	public:
		hemisphere_policy(void);
		virtual ~hemisphere_policy(void);
		
		struct context 
		{
			color4f low_color;
			color4f high_color;
		};

		static nstring get_name(void)	{	return nstring(_T("hemisphere_light"));}
		static void modify_drawing_policy_type(base_pass_policy_type* dp_type)
		{
			wostringstream ss;
			ss	<< _T("shader_d3d9/lighting/")
				<< get_name()
				<< _T(".hlsl");

			dp_type->m_Ambent_policy = ss.str();		
		}

		static void set_effect_parameter( d3d9_shading_effect* effect_ptr,const context& data)
		{
			effect_ptr->set_vector("TOP_COLOR",vector4(data.high_color.R,data.high_color.G,data.high_color.B,data.high_color.A));
			effect_ptr->set_vector("BOTTOM_COLOR",vector4(data.low_color.R,data.low_color.G,data.low_color.B,data.low_color.A));
		}
	};
}//namespace nexus
