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

	NPointLightComponent::NPointLightComponent(nexus::npoint_light_component::ptr nativeComp)
		: NLightComponent(nativeComp)
	{
	}

	npoint_light_component* NPointLightComponent::NativePtr::get()
	{
		return dynamic_cast<npoint_light_component*>(RawPtr);
	}

	NDirectionalLightComponent::NDirectionalLightComponent(nexus::ndirectional_light_component::ptr nativeComp)
		: NLightComponent(nativeComp)
	{
	}

	ndirectional_light_component* NDirectionalLightComponent::NativePtr::get()
	{
		return dynamic_cast<ndirectional_light_component*>(RawPtr);
	}

	NSpotLightComponent::NSpotLightComponent(nexus::nspot_light_component::ptr nativeComp)
		: NLightComponent(nativeComp)
	{
	}

	nspot_light_component* NSpotLightComponent::NativePtr::get()
	{
		return dynamic_cast<nspot_light_component*>(RawPtr);
	}

}//namespace NexusEngine