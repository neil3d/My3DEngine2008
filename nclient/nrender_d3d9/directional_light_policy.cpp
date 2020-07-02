#include "StdAfx.h"
#include <sstream>
#include "directional_light_policy.h"
#include "util.h"
#include "shading_effect.h"

namespace nexus
{
	directional_light_policy::directional_light_policy(void)
	{
	}

	directional_light_policy::~directional_light_policy(void)
	{
	}

	void directional_light_policy::modify_drawing_policy_type(light_drawing_policy_type* dp_type)
	{
		wostringstream ss;
		ss	<< _T("shader_d3d9/lighting/")
			<< get_name()
			<< _T(".hlsl");

		load_shder_source(ss.str(), dp_type->m_light_code);
	}

	void directional_light_policy::set_effect_parameter(d3d9_shading_effect* effect_ptr, const nrender_light_proxy* lgt)
	{
		lighting_policy::set_effect_parameter(effect_ptr, lgt);


		nASSERT(lgt->m_type == ELight_Directional);
		const ndirectional_light_proxy* dir_lgt = static_cast<const ndirectional_light_proxy*>(lgt);

		HRESULT hr;
		vector3 lgtDir = -dir_lgt->m_direction;	// todo: why?
		hr = effect_ptr->set_float_array_by_semantic("LGT_DIRECTION", lgtDir);				
	}
}//namespace nexus