#include "StdAfx.h"
#include "NSkeletalAnimPlayer.h"
#include "NResourceSkeletalAnimSet.h"

namespace NexusEngine
{
	NSkeletalAnimPlayer::NSkeletalAnimPlayer( nskel_anim_play::ptr nativeAnimPlay )
		: NativeObjectOwnedBySharedPtr(nativeAnimPlay)
	{

	}

	void NSkeletalAnimPlayer::Init( NResourceSkeletalAnimSet^ animSet, bool autoPlay )
	{
		NativePtr->init(animSet->GetSmartPtr(), autoPlay);
	}

	void NSkeletalAnimPlayer::SetAnim( String^ sequence )
	{
		pin_ptr<const wchar_t> nativeName = PtrToStringChars(sequence);
		NativePtr->set_anim(nativeName);
	}

	void NSkeletalAnimPlayer::PlayAnim( bool loop, float rate, float startTime )
	{
		NativePtr->play_anim(loop, rate, startTime);
	}

	void NSkeletalAnimPlayer::StopAnim()
	{
		NativePtr->stop_anim();
	}

	void NSkeletalAnimPlayer::SetPosition( float newTime, bool fireNotifies )
	{
		NativePtr->set_position(newTime, fireNotifies);
	}
}