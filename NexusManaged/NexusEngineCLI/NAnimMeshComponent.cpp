#include "StdAfx.h"
#include "NAnimMeshComponent.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NAnimMeshComponent::NAnimMeshComponent(nexus::nanim_mesh_component::ptr nativeComp):NMeshComponent(nativeComp)
	{
	}

	nanim_mesh_component* NAnimMeshComponent::NativePtr::get()
	{
		return dynamic_cast<nanim_mesh_component*>(RawPtr);
	}

	void NAnimMeshComponent::ResetResource(NResourceAnimMesh^ resMesh)
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

	void NAnimMeshComponent::ResetResource(NResourceLoc resMeshLoc)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		resMeshLoc.ToNative(nloc);

		NativePtr->reset_resource(nloc);

		END_NATIVE_GUARD
	}

	void NAnimMeshComponent::PlayAnim( bool loop, float rate, float startTime )
	{
		NativePtr->play_anim(loop, rate, startTime);
	}
	void NAnimMeshComponent::StopAnim()
	{
		const nmesh_anim_controller::ptr ac = NativePtr->get_anim_controller();
		if(ac)
		{
			ac->stop_anim();
		}
	}

	void NAnimMeshComponent::SetAnim( String^ seqName )
	{
		pin_ptr<const wchar_t> szSeqName = PtrToStringChars(seqName);
		NativePtr->set_anim(szSeqName);
	}

	void NAnimMeshComponent::SetPosition( float newTime, bool fireNotifies )
	{
		const nmesh_anim_controller::ptr ac = NativePtr->get_anim_controller();
		if(ac)
		{
			ac->set_position(newTime, fireNotifies);
		}
	}
}//namespace NexusEngine