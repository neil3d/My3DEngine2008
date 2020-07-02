#include "StdAfx.h"
#include "NEffectQuad.h"

namespace NexusEngine
{
	NEffectQuad::NEffectQuad(nexus::neffect_quad::ptr nativeObj):NEffectElement(nativeObj)
	{
	}

	neffect_quad* NEffectQuad::NativePtr::get()
	{
		return ndynamic_cast<neffect_quad>( m_smart_ptr->get() );
	}

	void NEffectQuad::Init(String^ name)
	{
		NEffectElement::Init(name);

		CREATE_DISTRIBUTION(m_size,(NativePtr->m_size),NDistributionVector2Const,NDistributionVector2Uniform,NDistributionVector2Curve)
		CREATE_DISTRIBUTION(m_color,(NativePtr->m_color),NDistributionColorConst,NDistributionColorUniform,NDistributionColorCurve)
		CREATE_DISTRIBUTION(m_revolution_vel,(NativePtr->m_revolution_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_angular_vel,(NativePtr->m_rotate_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_linear_vel,(NativePtr->m_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
	}
}//namespace NexusEngine