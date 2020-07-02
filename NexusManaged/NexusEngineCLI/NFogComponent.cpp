#include "StdAfx.h"
#include "NFogComponent.h"

namespace NexusEngine
{
	NFogComponent::NFogComponent(nexus::nfog_volume_component::ptr nativeComp)
		: NActorComponent(nativeComp)
	{
	}

	nfog_volume_component* NFogComponent::NativePtr::get()
	{
		return dynamic_cast<nfog_volume_component*>(RawPtr);
	}
}//namespace NexusEngine