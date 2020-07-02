#pragma once
#include "NActor.h"
#include "NResource.h"

namespace NexusEngine
{
	public ref class NAvatarActor : public NActor
	{
	public:
		NAvatarActor(nexus::navatar_actor::ptr nativeObj);

		void Create(NResourceLoc animSetLoc);
		void BindSkeletalMesh(String^ compName, NResourceLoc res_loc);
		void BindAttachment(String^ compName, String^ attachBone, NResourceLoc res_loc);
	
		// 动画播放接口
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

		property String^ AnimPlaying
		{
			String^ get()
			{
				return gcnew System::String( NativePtr->get_anim_playing() );
			}
		}

	protected:
		property navatar_actor* NativePtr
		{
			navatar_actor* get();
		}	
	};
}//namespace NexusEngine