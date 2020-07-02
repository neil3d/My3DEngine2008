#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	ref class NResourceSkeletalAnimSet;

	public ref class NSkeletalAnimPlayer : public NativeObjectOwnedBySharedPtr
	{
	public:
		NSkeletalAnimPlayer(nskel_anim_play::ptr nativeAnimPlay);

		property nskel_anim_play* NativePtr
		{
			nskel_anim_play* get()
			{
				return dynamic_cast<nskel_anim_play*>(RawPtr);
			}
		}

		void Init(NResourceSkeletalAnimSet^ animSet, bool autoPlay);

		void SetAnim(String^ sequence);

		void PlayAnim(bool loop, float rate, float startTime);

		void StopAnim();

		void SetPosition(float newTime, bool fireNotifies);

		property float CurrentAnimTimeLeft
		{
			float get()
			{
				return NativePtr->get_time_left();
			}
		}	

		property float CurrentAnimLength
		{
			float get()
			{
				return NativePtr->get_anim_length();
			}
		}	

		property UInt32 CurrentAnimFrame
		{
			UInt32 get()
			{
				return NativePtr->get_anim_frame();
			}
		}	

		property String^ CurrentAnimPlaying
		{
			String^ get()
			{
				return gcnew String(NativePtr->get_anim_playing());
			}
		}	

	};
}
