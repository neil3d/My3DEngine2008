#include "StdAfx.h"
#include "NMeshComponent.h"

namespace NexusEngine
{
	NMeshComponent::NMeshComponent(nexus::nmesh_component::ptr nativeComp):NPrimitiveComponent(nativeComp)
	{
	}

	nmesh_component* NMeshComponent::NativePtr::get()
	{
		return dynamic_cast<nmesh_component*>(RawPtr);
	}
}//namespace NexusEngine