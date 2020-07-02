#include "StdAfx.h"
#include "NMeshComponent.h"

namespace NexusEngine
{
	NMeshComponent::NMeshComponent(nexus::nmesh_component::ptr nativeComp):NPrimitiveComponent(nativeComp)
	{
	}

	nmesh_component* NMeshComponent::NativePtr::get()
	{
		return dynamic_cast<nmesh_component*>(RawPtr);
	}

	void NMeshComponent::ReplaceMaterial(int lod, int mtlID, NMaterial^ mtl)
	{
		NativePtr->replace_material(lod, mtlID, mtl->GetSmartPtr());
	}
}//namespace NexusEngine