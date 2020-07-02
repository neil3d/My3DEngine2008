#ifndef _NEXUS_SHADOW_POLICY_BASE_H_
#define _NEXUS_SHADOW_POLICY_BASE_H_
#include "light_drawing_policy_type.h"

namespace nexus
{
	class d3d9_shading_effect;

	class shadow_policy_base
	{
	public:
		shadow_policy_base(void)	{	}
		virtual ~shadow_policy_base(void)	{	}

		static nstring get_name(void)	{	return nstring(_T("light_none"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type)	{}

		virtual void set_effect_parameter(d3d9_shading_effect* effect_ptr) = 0;
	};

	class noshadow
		: public shadow_policy_base
	{
	public:
		noshadow(void)	{}
		virtual ~noshadow(void)	{}

		static nstring get_name(void)	{	return nstring(_T("noshadow"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type);
		virtual void set_effect_parameter(d3d9_shading_effect* effect_ptr)	{}
	};
}//namespace nexus
#endif //_NEXUS_SHADOW_POLICY_BASE_H_