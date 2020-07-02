#pragma once
#include "NActorComponent.h"
#include "NRenderLight.h"

namespace NexusEngine
{
	public ref class NLightComponent
		: public NActorComponent
	{
	public:
		void CreatePointLight();
		NPointLightProxy^ GetPointLight()	
		{
			return (NPointLightProxy^)m_lgtProxy;
		}

		void CreateDirectionalLight();
		NDirectionalLightProxy^ GetDirectionalLight()
		{
			return (NDirectionalLightProxy^)m_lgtProxy;
		}


	protected:
		NRenderLightProxy^	m_lgtProxy;

		//-- Native Wrapper
	public:
		NLightComponent(nexus::nlight_component::ptr nativeComp);

		property nlight_component* NativePtr
		{
			nlight_component* get();
		}
	};
}//namespace NexusEngine