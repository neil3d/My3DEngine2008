#pragma once
#include "NEffectElement.h"
#include "NDistributionAll.h"

namespace NexusEngine
{
	public ref class NEffectQuad : public NEffectElement
	{
	public:
		NEffectQuad(nexus::neffect_quad::ptr nativeObj);

		[Category("EffectQuad")]
		property enum class EsfxFaceType FaceType
		{
			enum class EsfxFaceType get()			{	return (enum class EsfxFaceType)NativePtr->m_face_type;}
			void set(enum class EsfxFaceType val)	{	NativePtr->m_face_type = (nexus::ESFX_FaceType)val;}
		}

		[Category("EffectQuad")]
		property int NumTailFaces
		{
			int get()			{	return NativePtr->m_num_tail_faces; }
			void set(int val)	{	NativePtr->m_num_tail_faces = val; }
		}

		[Category("EffectQuad")]
		property float TailInterval
		{
			float get()			{	return NativePtr->m_tail_interval; }
			void set(float val)	{	NativePtr->m_tail_interval = val; }
		}

		[Category("EffectQuad")]
		property NDistributionVector2^ DSize
		{
			NDistributionVector2^ get()
			{
				return m_size;
			}
			void set(NDistributionVector2^ val)
			{
				NativePtr->m_size = val->GetNativeSmartPtr();
				m_size = val;
			}
		}

		[Category("EffectQuad")]
		property NDistributionColor^ Colors
		{
			NDistributionColor^ get()
			{
				return m_color;
			}
			void set(NDistributionColor^ val)
			{
				NativePtr->m_color = val->GetNativeSmartPtr();
				m_color = val;
			}
		}

		[Category("EffectQuad")]
		property NDistributionVector3^ LinearVel
		{
			NDistributionVector3^ get()
			{
				return m_linear_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_vel = val->GetNativeSmartPtr();
				m_linear_vel = val;
			}
		}

		[Category("EffectQuad")]
		property NDistributionVector3^ RevolutionVel
		{
			NDistributionVector3^ get()
			{
				return m_revolution_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_revolution_vel= val->GetNativeSmartPtr();
				m_revolution_vel = val;
			}
		}

		[Category("EffectQuad")]
		property NDistributionVector3^ AngularVel
		{
			NDistributionVector3^ get()
			{
				return m_angular_vel;
			}
			void set(NDistributionVector3^ val)
			{
				NativePtr->m_rotate_vel= val->GetNativeSmartPtr();
				m_angular_vel = val;
			}
		}

		virtual void Init(String^ name) override;
	protected:
		property neffect_quad* NativePtr
		{
			neffect_quad* get();
		}

	protected:
		//尺寸
		NDistributionVector2^	m_size;
		//线速度
		NDistributionVector3^	m_linear_vel;
		//角速度
		NDistributionVector3^	m_revolution_vel;
		//自转角速度
		NDistributionVector3^	m_angular_vel;
		//色彩
		NDistributionColor^		m_color;
	};
}//namespace NexusEngine