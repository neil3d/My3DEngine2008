#pragma once
#include "NativeObject.h"
#include "math/Color4.h"
#include "math/Vector3.h"

namespace NexusEngine
{
	public enum class ELightType : System::Int32
	{
		None		=	nexus::ELight_None,
		Point		=	nexus::ELight_Point,
		Directional	=	nexus::ELight_Directional,		
	};

	public ref class NRenderLightProxy
		: public NativeObjectBorrowed
	{
	public:		
		property enum class ELightType LightType
		{
			enum class ELightType get()
			{
				return (enum class ELightType)NativePtr->m_type;
			}
			void set(enum class ELightType val)
			{
				NativePtr->m_type = (nexus::ELightType)val;
			}
		}

		property Color4f AmbientColor
		{
			Color4f get()
			{
				return Color4f::FromNative(NativePtr->m_ambient_color);
			}
			void set(Color4f val)
			{
				NativePtr->m_ambient_color = val.ToNative();
			}
		}

		property Color4f DiffuseColor
		{
			Color4f get()
			{
				return Color4f::FromNative(NativePtr->m_diffuse_color);
			}
			void set(Color4f val)
			{
				NativePtr->m_diffuse_color = val.ToNative();
			}
		}

		//-- Native Wrapper
	public:
		NRenderLightProxy(nexus::nrender_light_proxy* nativeLgt);	
	protected:
		property nexus::nrender_light_proxy* NativePtr
		{
			nexus::nrender_light_proxy* get() new;			
		}
	};

	public ref class NPointLightProxy 
		: public NRenderLightProxy
	{
	public:
		property Vector3 Position
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_position);
			}
			void set(Vector3 val)
			{
				NativePtr->m_position = val.ToNative();
			}
		}

		property Vector3 Attenuation
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_attenuation);
			}
			void set(Vector3 val)
			{
				NativePtr->m_attenuation = val.ToNative();
			}
		}

		//-- Native Wrapper
	public:
		NPointLightProxy(nexus::npoint_light_proxy* nativeLgt);	
	protected:
		property nexus::npoint_light_proxy* NativePtr
		{
			nexus::npoint_light_proxy* get() new;			
		}
	};

	public ref class NDirectionalLightProxy 
		: public NRenderLightProxy
	{
	public:
		property Vector3 Direction
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_direction);
			}
			void set(Vector3 val)
			{
				NativePtr->m_direction = val.ToNative();
			}
		}	

		//-- Native Wrapper
	public:
		NDirectionalLightProxy(nexus::ndirectional_light_proxy* nativeLgt);	
	protected:
		property nexus::ndirectional_light_proxy* NativePtr
		{
			nexus::ndirectional_light_proxy* get() new;			
		}
	};
}//namespace NexusEngine