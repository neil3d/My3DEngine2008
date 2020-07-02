#include "StdAfx.h"
#include "NLightComponent.h"

namespace NexusEngine
{
	NLightComponent::NLightComponent(nexus::nlight_component::ptr nativeComp)
		: NActorComponent(nativeComp)
	{
	}

	nlight_component* NLightComponent::NativePtr::get()
	{
		return dynamic_cast<nlight_component*>(RawPtr);
	}

	void NLightComponent::CreatePointLight()
	{
		NativePtr->create_point_light();
		
		m_lgtProxy = gcnew NPointLightProxy(NativePtr->get_point_light());
	}

	void NLightComponent::CreateDirectionalLight()
	{
		NativePtr->create_directional_light();
		
		m_lgtProxy = gcnew NDirectionalLightProxy(NativePtr->get_directional_light());
	}
}//namespace NexusEngine