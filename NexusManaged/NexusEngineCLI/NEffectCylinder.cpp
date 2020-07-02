#include "stdafx.h"
#include "NEffectCylinder.h"

namespace NexusEngine
{
	//==============================================================================================================================
	NEffectCylinder::NEffectCylinder(nexus::neffect_cylinder::ptr nativeObj):NEffectElement(nativeObj)
	{
	}

	void NEffectCylinder::Init( String^ name )
	{
		NEffectElement::Init(name);

		CREATE_DISTRIBUTION(m_num_circle,(NativePtr->m_num_circle),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_height,(NativePtr->m_height),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_top_radius,(NativePtr->m_top_radius),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_bottom_radius,(NativePtr->m_bottom_radius),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_top_radius_inc,(NativePtr->m_div_top_radius_inc),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_bottom_radius_inc,(NativePtr->m_div_bottom_radius_inc),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_top_radius_shrink,(NativePtr->m_div_top_radius_shrink),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_bottom_radius_shrink,(NativePtr->m_div_bottom_radius_shrink),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_top_rise,(NativePtr->m_div_top_rise),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_div_bottom_rise,(NativePtr->m_div_bottom_rise),NDistributionFloatConst,NDistributionFloatUniform,NDistributionFloatCurve)
		CREATE_DISTRIBUTION(m_top_color,(NativePtr->m_top_color),NDistributionColorConst,NDistributionColorUniform,NDistributionColorCurve)
		CREATE_DISTRIBUTION(m_bottom_color,(NativePtr->m_bottom_color),NDistributionColorConst,NDistributionColorUniform,NDistributionColorCurve)
	}
	//==============================================================================================================================
};

