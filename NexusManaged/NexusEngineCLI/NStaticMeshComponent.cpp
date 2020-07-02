#include "StdAfx.h"
#include "NStaticMeshComponent.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NStaticMeshComponent::NStaticMeshComponent(nexus::nstatic_mesh_component::ptr nativeComp):NMeshComponent(nativeComp)
	{
	}

	nstatic_mesh_component* NStaticMeshComponent::NativePtr::get()
	{
		return dynamic_cast<nstatic_mesh_component*>(RawPtr);
	}

	void NStaticMeshComponent::ResetResource(NResourceStaticMesh^ resMesh)
	{
		if (resMesh == nullptr)
		{
			throw gcnew System::ArgumentException("Bad Static Mesh Resource Pointer.");
		}
		else
		{
			NativePtr->reset_resource(resMesh->GetSmartPtr());
		}
	}

	void NStaticMeshComponent::ResetResource(NResourceLoc resMeshLoc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		resMeshLoc.ToNative(nloc);

		NativePtr->reset_resource(nloc);

		END_NATIVE_GUARD
	}
}//namespace NexusEngine