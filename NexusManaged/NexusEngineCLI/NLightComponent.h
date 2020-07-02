#pragma once
#include "NActorComponent.h"
#include "math/Color4.h"
#include "math/Color3.h"
#include "math/Vector3.h"

namespace NexusEngine
{
	public enum class ELightType : System::Int32
	{
		None		=	nexus::ELight_None,
		Point		=	nexus::ELight_Point,
		Directional	=	nexus::ELight_Directional,	
		Spot		=   nexus::Elight_Spot
	};

	public ref class NLightComponent
		: public NActorComponent
	{
	public:
		property ELightType LightType
		{
			ELightType get()
			{
				return (ELightType)(NativePtr->m_type);
			}

			void set(ELightType val)
			{
			//	NativePtr->m_type = (nexus::ELightType)val;
			}
		}

		property Color3 Color
		{
			Color3 get()
			{
				return Color3(NativePtr->m_color.R*255,NativePtr->m_color.G*255,NativePtr->m_color.B*255);
			}

			void set(Color3 val)
			{
				NativePtr->m_color.R = (float)val.R/255.0f;
				NativePtr->m_color.G = (float)val.G/255.0f;
				NativePtr->m_color.B = (float)val.B/255.0f;
			}
		}

		DECLARE_SINGLE_PROPERTY(float, Brightness,m_color.A)
		DECLARE_SINGLE_PROPERTY(float, ShadowSoftness,m_shadow_softness)
		DECLARE_SINGLE_PROPERTY(bool, bProjectShadow,project_shadow)

		//-- Native Wrapper
	public:
		NLightComponent(nexus::nlight_component::ptr nativeComp);

		property nlight_component* NativePtr
		{
			nlight_component* get();
		}
	};

	public ref class NPointLightComponent
		: public NLightComponent
	{
	public:
		DECLARE_SINGLE_PROPERTY(float, LightRadius,m_attenuation.x)
		DECLARE_SINGLE_PROPERTY(float, AttenuationPower,m_attenuation.y)

		//-- Native Wrapper
	public:
		NPointLightComponent(nexus::npoint_light_component::ptr nativeComp);

		property npoint_light_component* NativePtr
		{
			npoint_light_component* get();
		}
	};

	public ref class NSpotLightComponent
		: public NLightComponent
	{
	public:
		DECLARE_SINGLE_PROPERTY(float, LightRadius,m_attenuation.x)
		DECLARE_SINGLE_PROPERTY(float, AttenuationPower,m_attenuation.y)
		DECLARE_SINGLE_PROPERTY(float, InnerConeAngle,inner_cone_angle)
		DECLARE_SINGLE_PROPERTY(float, OuterConeAngle,outer_cone_angle)

		//-- Native Wrapper
	public:
		NSpotLightComponent(nexus::npoint_light_component::ptr nativeComp);

		property nspot_light_component* NativePtr
		{
			nspot_light_component* get();
		}
	};

	public ref class NDirectionalLightComponent
		: public NLightComponent
	{
	public:
		//-- Native Wrapper
	public:
		NDirectionalLightComponent(nexus::ndirectional_light_component::ptr nativeComp);

		property ndirectional_light_component* NativePtr
		{
			ndirectional_light_component* get();
		}
	};
}//namespace NexusEngine