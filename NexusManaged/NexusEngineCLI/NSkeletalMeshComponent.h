#pragma once
#include "NMeshComponent.h"
#include "NResourceSkeletalMesh.h"

namespace NexusEngine
{
	ref class NSkeletalAnimPlayer;

	public ref class NSkeletalMeshComponent : public NMeshComponent
	{
	public:		
		//!	指定关联的资源
		void ResetResource(NResourceSkeletalMesh^ resMesh, bool cpuSkin);

		//!	指定关联的资源, 内部执行Load
		void ResetResource(NResourceLoc resMeshLoc, bool cpuSkin);

		//! 重置AnimController为class nskel_anim_play
		NSkeletalAnimPlayer^ ResetSkeletalAnimPlayer();

		//-- Native Wrapper
	public:
		NSkeletalMeshComponent(nexus::nskeletal_mesh_component::ptr nativeComp);
	private:
		property nskeletal_mesh_component* NativePtr
		{
			nskeletal_mesh_component* get();
		}

	};
}