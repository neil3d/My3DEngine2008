#include "StdAfx.h"
#include "NavatarActor.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NAvatarActor::NAvatarActor(nexus::navatar_actor::ptr nativeObj):NActor(nativeObj)
	{
	}

	navatar_actor* NAvatarActor::NativePtr::get()
	{
		return dynamic_cast<navatar_actor*>(RawPtr);
	}

	void NAvatarActor::Create(NResourceLoc animSetLoc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		animSetLoc.ToNative(nloc);

		NativePtr->create(nloc);

		END_NATIVE_GUARD
	}

	void NAvatarActor::BindSkeletalMesh(String^ compName, NResourceLoc res_loc)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		res_loc.ToNative(nloc);

		pin_ptr<const wchar_t> szCompName = PtrToStringChars(compName);
		NativePtr->bind_skeletal_mesh(szCompName, nloc);

		END_NATIVE_GUARD
	}

	void NAvatarActor::BindAttachment(String^ compName, String^ attachBone, NResourceLoc res_loc)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		res_loc.ToNative(nloc);

		pin_ptr<const wchar_t> szCompName = PtrToStringChars(compName);
		pin_ptr<const wchar_t> szAttachBone = PtrToStringChars(attachBone);
		NativePtr->bind_attachment(szCompName, szAttachBone, nloc);

		END_NATIVE_GUARD
	}


	void NAvatarActor::SetAnim( String^ sequence )
	{
		pin_ptr<const wchar_t> nativeSeqName = PtrToStringChars(sequence);
		NativePtr->set_anim(nativeSeqName);
	}

	void NAvatarActor::PlayAnim( bool loop, float rate, float startTime )
	{
		NativePtr->play_anim(loop, rate, startTime);
	}

	void NAvatarActor::StopAnim()
	{
		NativePtr->stop_anim();
	}

	void NAvatarActor::SetPosition( float newTime, bool fireNotifies )
	{
		NativePtr->set_position(newTime, fireNotifies);
	}

}//namespace NexusEngine

