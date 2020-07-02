/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"
#include "NMaterial.h"

namespace NexusEngine
{
	/**
	 *	Mesh类组件的基类,包装nexus::nmesh_component对象
	*/
	public ref class NMeshComponent : public NPrimitiveComponent
	{
	public:
		//!	指定材质替代resource对象所包含的材质
		void ReplaceMaterial(int lod, int mtlID, NMaterial^ mtl);

		//-- Native wrapper
	public:
		NMeshComponent(nexus::nmesh_component::ptr nativeComp);
	protected:
		property nmesh_component* NativePtr
		{
			nmesh_component* get();
		}		
	};
}//namespace NexusEngine