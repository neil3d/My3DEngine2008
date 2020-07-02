#pragma once
#include "NMeshComponent.h"
#include "NResourceAnimMesh.h"

namespace NexusEngine
{
	public ref class NAnimMeshComponent : public NMeshComponent
	{
	public:		
		//!	指定关联的资源
		void ResetResource(NResourceAnimMesh^ resMesh);

		//!	指定关联的资源, 内部执行Load
		void ResetResource(NResourceLoc resMeshLoc);

		void SetAnim(String^ seqName);
		void PlayAnim(bool loop, float rate, float startTime);
		void StopAnim();
		void SetPosition(float newTime, bool fireNotifies);

		property float CurrentAnimTimeLeft
		{
			float get()
			{
				const nmesh_anim_controller::ptr ac = NativePtr->get_anim_controller();
				if(ac)
				{
					return ac->get_time_left();
				}
				return 0;
			}
		}

		property float CurrentAnimLength
		{
			float get()
			{
				const nmesh_anim_controller::ptr ac = NativePtr->get_anim_controller();
				if(ac)
				{
					return ac->get_anim_length();
				}
				return 0;
			}
		}

		property String^ AnimPlaying
		{
			String^ get()
			{
				return gcnew System::String( NativePtr->get_anim_playing() );
			}
		}

		//-- Native Wrapper
	public:
		NAnimMeshComponent(nexus::nanim_mesh_component::ptr nativeComp);
	private:
		property nanim_mesh_component* NativePtr
		{
			nanim_mesh_component* get();
		}
	};
}//namespace NexusEngine