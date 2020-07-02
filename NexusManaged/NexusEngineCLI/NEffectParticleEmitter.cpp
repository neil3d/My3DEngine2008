#include "StdAfx.h"
#include "NEffectParticleEmitter.h"
namespace NexusEngine
{
	NEffectParticleEmitter::NEffectParticleEmitter( nexus::neffect_particle_emitter::ptr nativeObj ):NEffectElement(nativeObj)
	{

	}

	void NEffectParticleEmitter::Init( String^ name )
	{
		NEffectElement::Init(name);

		CREATE_DISTRIBUTION(m_size,(NativePtr->m_size),NDistributionVector2Const,NDistributionVector2Uniform,NDistributionVector2Curve)
		CREATE_DISTRIBUTION(m_color,(NativePtr->m_color),NDistributionColorConst,NDistributionColorUniform,NDistributionColorCurve)
		CREATE_DISTRIBUTION(m_revolution_vel,(NativePtr->m_revolution_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_angular_vel,(NativePtr->m_rotate_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_linear_vel,(NativePtr->m_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_emitter_angular,(NativePtr->m_emitter_rot),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_emitter_revolution,(NativePtr->m_emitter_revo),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
		CREATE_DISTRIBUTION(m_emitter_linear,(NativePtr->m_emitter_vel),NDistributionVector3Const,NDistributionVector3Uniform,NDistributionVector3Curve)
	}

	neffect_particle_emitter* NEffectParticleEmitter::NativePtr::get()
	{
		return ndynamic_cast<neffect_particle_emitter>( m_smart_ptr->get() );
	}
}