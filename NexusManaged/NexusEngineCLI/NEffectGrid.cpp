#include "StdAfx.h"
#include "NEffectGrid.h"

namespace NexusEngine
{
	NEffectGrid::NEffectGrid( nexus::neffect_grid::ptr nativeObj ):NEffectElement(nativeObj)
	{
		
	}

	void NEffectGrid::Init(String^ name)
	{
		NEffectElement::Init(name);

		CREATE_DISTRIBUTION(m_size,(NativePtr->m_size),NDistributionVector2Const,NDistributionVector2Uniform,NDistributionVector2Curve)
		CREATE_DISTRIBUTION(m_color,(NativePtr->m_color),NDistributionColorConst,NDistributionColorUniform,NDistributionColorCurve)
		CREATE_DISTRIBUTION(m_revolution_vel,(NativePtr->m_revolution_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_angular_vel,(NativePtr->m_rotate_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_linear_vel,(NativePtr->m_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_wave_power,(NativePtr->m_wave_power),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
	}	
}

