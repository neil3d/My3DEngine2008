#include "StdAfx.h"
#include "NWaterComponent.h"

namespace NexusEngine
{
	NWaterComponent::NWaterComponent(nexus::nwater_component::ptr nativeComp)
		: NActorComponent(nativeComp)
	{
	}



	nexus::nwater_component* NWaterComponent::NativePtr::get()
	{
		return dynamic_cast<nexus::nwater_component*>(RawPtr);
	}
}//namespace NexusEngine