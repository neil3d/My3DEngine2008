#include "StdAfx.h"
#include <sstream>
#include "point_light_policy.h"
#include "util.h"
#include "shading_effect.h"

namespace nexus
{
	point_light_policy::point_light_policy(void)
	{
	}

	point_light_policy::~point_light_policy(void)
	{
	}

	void point_light_policy::modify_drawing_policy_type(light_drawing_policy_type* dp_type)
	{
		wostringstream ss;
		ss	<< _T("shader_d3d9/lighting/")
			<< get_name()
			<< _T(".hlsl");

		dp_type->m_light_policy = ss.str();		
	}

	void point_light_policy::set_effect_parameter(d3d9_shading_effect* effect_ptr, const nrender_light_proxy* lgt)
	{
		lighting_policy::set_effect_parameter(effect_ptr, lgt);
		
		
		nASSERT(lgt->m_type == ELight_Point);
		const npoint_light_proxy* point_lgt = static_cast<const npoint_light_proxy*>(lgt);

		HRESULT hr;
		hr = effect_ptr->set_float_array_by_semantic("LGT_Position", point_lgt->m_position);		
		hr = effect_ptr->set_float_array_by_semantic("LGT_Attenuation", point_lgt->m_attenuation);
	}

	void lighting_policy::set_effect_parameter(d3d9_shading_effect* effect_ptr, const nrender_light_proxy* lgt)
	{
		nASSERT(effect_ptr != NULL);
		nASSERT(lgt != NULL);

		HRESULT hr;
		hr = effect_ptr->set_float_array_by_semantic("LGT_DiffuseColor", lgt->m_diffuse_color);
		hr = effect_ptr->set_float_array_by_semantic("LGT_AmbientColor", lgt->m_ambient_color);
	}
}//namespace nexus