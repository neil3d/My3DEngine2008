#include "StdAfx.h"
#include "NSkeletalMeshComponent.h"
#include "NWrapperException.h"
#include "NSkeletalAnimPlayer.h"

namespace NexusEngine
{
	NSkeletalMeshComponent::NSkeletalMeshComponent(nexus::nskeletal_mesh_component::ptr nativeComp):NMeshComponent(nativeComp)
	{
	}

	nskeletal_mesh_component* NSkeletalMeshComponent::NativePtr::get()
	{
		return dynamic_cast<nskeletal_mesh_component*>(RawPtr);
	}

	void NSkeletalMeshComponent::ResetResource( NResourceSkeletalMesh^ resMesh, bool cpuSkin)
	{
		if (resMesh == nullptr)
		{
			throw gcnew System::ArgumentException("Bad Static Mesh Resource Pointer.");
		}
		else
		{
			NativePtr->reset_resource(resMesh->GetSmartPtr(),cpuSkin);
		}
	}

	void NSkeletalMeshComponent::ResetResource( NResourceLoc resMeshLoc, bool cpuSkin )
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		resMeshLoc.ToNative(nloc);

		NativePtr->reset_resource(nloc, cpuSkin);

		END_NATIVE_GUARD
	}

	NSkeletalAnimPlayer^ NSkeletalMeshComponent::ResetSkeletalAnimPlayer()
	{
		nskel_anim_controller::ptr nativeController = NativePtr->reset_anim_controller(_T("nskel_anim_play"));
		return gcnew NSkeletalAnimPlayer(nativeController);
	}
}//namespace NexusEngine
