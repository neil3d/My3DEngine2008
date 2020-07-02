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

		void PlayAnim(String^ seqName, bool loop, float startTime);

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