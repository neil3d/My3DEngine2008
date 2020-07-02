#include "StdAfx.h"
#include "NSpeedTreeComponent.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NSpeedTreeComponent::NSpeedTreeComponent(nexus::nspeed_tree_component::ptr nativeComp)
		:NPrimitiveComponent(nativeComp)
	{
	}

	nspeed_tree_component* NSpeedTreeComponent::NativePtr::get()
	{
		return dynamic_cast<nspeed_tree_component*>(RawPtr);
	}

	void NSpeedTreeComponent::Create(NResourceLoc loc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc.ToNative(nloc);

		NativePtr->create(nloc);

		END_NATIVE_GUARD
	}
}//namespace NexusEngine