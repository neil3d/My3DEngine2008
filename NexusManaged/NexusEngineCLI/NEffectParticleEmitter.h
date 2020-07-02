#pragma once
#include "NEffectElement.h"
#include "NDistributionAll.h"

namespace NexusEngine
{
	//粒子发射器的形状
	public enum class EsfxEmitterShape
	{
		SphereShape			= ESFX_SphereShape,
		BoxShape			= ESFX_BoxShape,
		ConeShape			= ESFX_ConeShape,
	};

	//粒子的运动形式
	public enum class EsfxMotionType
	{
		FreeMotion			= ESFX_FreeMotion,
		TrackMotion			= ESFX_TrackMotion,	
		ExplodeMotion		= ESFX_ExplodeMotion,
		AssembleMotion		= ESFX_AssembleMotion,
	};

	public ref class NEffectParticleEmitter : public NEffectElement
	{
	public:
		NEffectParticleEmitter(nexus::neffect_particle_emitter::ptr nativeObj);

		[Category("粒子"), Description("粒子的面片类型")]
		property enum class EsfxFaceType FaceType
		{
			enum class EsfxFaceType get()			{	return (enum class EsfxFaceType)NativePtr->m_config.m_face_type;}
			void set(enum class EsfxFaceType val)	{	NativePtr->m_config.m_face_type = (nexus::ESFX_FaceType)val;}
		}

		[Category("粒子"), Description("粒子的初始位置")]
		property Vector3 InitPos 
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_pos);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_pos = val.ToNative();
				NativePtr->m_config.m_init_position=val.ToNative();
			}
		}

		[Category("粒子"), Description("粒子的初始速度，对轨迹行为的粒子无效")]
		property Vector3 InitSpeed 
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_init_speed);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_init_speed=val.ToNative();
			}
		}

		[Category("粒子"), Description("粒子的初始自转")]
		property Vector3 InitRot
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_rot);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_rot = val.ToNative();
				NativePtr->m_config.m_init_rot=val.ToNative();
			}
		}

		[Category("粒子"), Description("粒子的初始公转")]
		property Vector3 InitRevolution
		{
			virtual Vector3 get() override
			{
				return Vector3::FromNative(NativePtr->m_init_revolution);
			}
			virtual void set(Vector3 val) override
			{
				NativePtr->m_init_revolution = val.ToNative();
				NativePtr->m_config.m_init_revo = val.ToNative();
			}
		}

		[Category("粒子"), Description("粒子运行行为类型")]
		property enum class EsfxMotionType MotionType
		{
			enum class EsfxMotionType get()			{	return (enum class EsfxMotionType)NativePtr->m_config.m_motion;}
			void set(enum class EsfxMotionType val)	{	NativePtr->m_config.m_motion = (nexus::ESFX_MotionType)val;}
		}

		[Category("粒子"), Description("粒子大小随时间变化值")]
		property NDistributionVector2^ DSize
		{
			NDistributionVector2^ get()
			{
				return m_size;
			}
			void set(NDistributionVector2^ val)
			{
				m_size = val;
				NativePtr->m_size = m_size->GetNativeSmartPtr();
				NativePtr->m_config.m_size= m_size->GetNativeSmartPtr();				
			}
		}

		[Category("粒子"), Description("粒子的色彩随时间变化值")]
		property NDistributionColor^ Colors
		{
			NDistributionColor^ get()
			{
				return m_color;
			}
			void set(NDistributionColor^ val)
			{
				m_color = val;
				NativePtr->m_color = val->GetNativeSmartPtr();
				NativePtr->m_config.m_color = val->GetNativeSmartPtr();
			}
		}

		[Category("粒子"), Description("粒子的移动速度随时间变化值")]
		property NDistributionVector3^ LinearVel
		{
			NDistributionVector3^ get()
			{
				return m_linear_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_vel = val->GetNativeSmartPtr();
				NativePtr->m_config.m_vel=val->GetNativeSmartPtr();
				m_linear_vel = val;
			}
		}

		[Category("粒子"), Description("粒子的公转速度随时间变化值")]
		property NDistributionVector3^ RevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_revolution_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_revolution_vel= val->GetNativeSmartPtr();
				NativePtr->m_config.m_revolution_vel= val->GetNativeSmartPtr();
				m_revolution_vel = val;
			}
		}

		[Category("粒子"), Description("粒子的自转速度随时间变化值")]
		property NDistributionVector3^ AngularVel
		{
			NDistributionVector3^ get()
			{
				return m_angular_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_rotate_vel= val->GetNativeSmartPtr();
				NativePtr->m_config.m_rotate_vel= val->GetNativeSmartPtr();
				m_angular_vel = val;
			}
		}

		[Category("粒子"), Description("粒子的最大随机漂移")]
		property float MaxDrift
		{
			float get()
			{
				return NativePtr->m_config.m_max_drift;
			}
			void set(float val)
			{
				NativePtr->m_config.m_max_drift= val;
			}
		}

		[Category("粒子"), Description("粒子的最小随机漂移")]
		property float MinDrift
		{
			float get()
			{
				return NativePtr->m_config.m_min_drift;
			}
			void set(float val)
			{
				NativePtr->m_config.m_min_drift= val;
			}
		}

		[Category("粒子"), Description("重力加速度，0表示不受重力影响")]
		property float Gravity
		{
			virtual float get() override
			{
				return NativePtr->m_gravity;
			}
			virtual void set(float val) override
			{
				NativePtr->m_gravity = val;
				NativePtr->m_config.m_gravity=val;
			}
		}

		[Category("粒子"), Description("空气阻尼")]
		property float Damp
		{
			float get()
			{
				return NativePtr->m_config.m_damp;
			}
			void set(float val)
			{
				NativePtr->m_config.m_damp=val;
			}
		}

		[Category("粒子"), Description("粒子的生命周期")]
		property float LifeTime
		{
			float get()
			{
				return NativePtr->m_config.m_particle_life;
			}
			void set(float val)
			{
				NativePtr->m_config.m_particle_life=val;
			}
		}

		[Category("粒子发射器"), Description("每秒发射的粒子数量")]
		property int EmitCountPerSecond
		{
			int get()
			{
				return NativePtr->m_config.m_emit_speed;
			}
			void set(int val)
			{
				NativePtr->m_config.m_emit_speed=val;
			}
		}

		[Category("粒子发射器"), Description("最大粒子数量")]
		property int ParticleCount
		{
			int get()
			{
				return NativePtr->m_config.m_max_particle_count;
			}
			void set(int val)
			{
				NativePtr->m_config.m_max_particle_count=val;
			}
		}

		[Category("粒子发射器"), Description("初始时一次生成的粒子数量")]
		property int InitParticleCount
		{
			int get()
			{
				return NativePtr->m_config.m_init_particle_count;
			}
			void set(int val)
			{
				NativePtr->m_config.m_init_particle_count=val;
			}
		}

		[Category("粒子发射器"), Description("发射器形状参数")]
		property Vector3 EmitterShapeParams
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_emitter_shape_parameter);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_emitter_shape_parameter= val.ToNative();
			}
		}

		[Category("粒子发射器"), Description("粒子发射器形状")]
		property enum class EsfxEmitterShape EmitterShape
		{
			enum class EsfxEmitterShape get()			{	return (enum class EsfxEmitterShape)NativePtr->m_config.m_emitter_shape;}
			void set(enum class EsfxEmitterShape val)	{	NativePtr->m_config.m_emitter_shape = (nexus::ESFX_EmitterShape)val;}
		}

		[Category("粒子发射器"), Description("粒子发射器初始位置")]
		property Vector3 EmitterInitPos
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_config.m_emitter_init_pos);
			}
			void set(Vector3 val)
			{
				NativePtr->m_config.m_emitter_init_pos= val.ToNative();
			}
		}

		[Category("粒子发射器"), Description("粒子发射器的移动速度随时间变化值")]
		property NDistributionVector3^ EmitterLinearVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_linear;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_vel = val->GetNativeSmartPtr();
				m_emitter_linear = val;
			}
		}

		[Category("粒子发射器"), Description("粒子发射器的公转速度随时间变化值")]
		property NDistributionVector3^ EmitterRevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_revolution;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_revo= val->GetNativeSmartPtr();
				m_emitter_revolution = val;
			}
		}

		[Category("粒子发射器"), Description("粒子发射器的自转速度随时间变化值")]
		property NDistributionVector3^ EmitterAngularVel
		{
			NDistributionVector3^ get()
			{
				return m_emitter_angular;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_emitter_rot= val->GetNativeSmartPtr();
				m_emitter_angular = val;
			}
		}

		virtual void Init(String^ name) override;
	protected:
		property neffect_particle_emitter* NativePtr
		{
			neffect_particle_emitter* get();
		}

	protected:
		//粒子尺寸
		NDistributionVector2^	m_size;
		//粒子线速度
		NDistributionVector3^	m_linear_vel;
		//粒子角速度
		NDistributionVector3^	m_revolution_vel;
		//粒子自转角速度
		NDistributionVector3^	m_angular_vel;
		//粒子色彩
		NDistributionColor^		m_color;
		//发射器速度
		NDistributionVector3^	m_emitter_linear;
		//发射器公转角速度
		NDistributionVector3^	m_emitter_revolution;
		//发射器自转角速度
		NDistributionVector3^	m_emitter_angular;
	};
}//namespace NexusEngine