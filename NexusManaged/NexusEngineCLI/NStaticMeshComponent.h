/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NMeshComponent.h"
#include "NResourceStaticMesh.h"

namespace NexusEngine
{
	/**
	 *	Static Mesh组件
	*/
	public ref class NStaticMeshComponent : public NMeshComponent
	{
	public:		
		//!	指定关联的资源
		void ResetResource(NResourceStaticMesh^ resMesh);

		//!	指定关联的资源, 内部执行Load
		void ResetResource(NResourceLoc resMeshLoc);

		//-- Native Wrapper
	public:
		NStaticMeshComponent(nexus::nstatic_mesh_component::ptr nativeComp);
	private:
		property nstatic_mesh_component* NativePtr
		{
			nstatic_mesh_component* get();
		}
	};
}//namespace NexusEngine