#include "StdAfx.h"
#include "NShapeComponent.h"

namespace NexusEngine
{
	NShapeComponent::NShapeComponent(nexus::nshape_component::ptr nativeComp)
		: NMeshComponent(nativeComp)
	{
	}

	nshape_component* NShapeComponent::NativePtr::get()
	{
		return dynamic_cast<nshape_component*>(RawPtr);
	}

	void NShapeComponent::CreateBox(float width, float height, float depth)
	{
		NativePtr->create_box(width, height, depth);
	}

	void NShapeComponent::CreateSphere(float radius, int numRings, int numSections)
	{
		NativePtr->create_sphere(radius, numRings, numSections);
	}

	void NShapeComponent::SetMaterial(NMtlBase^ mtl)
	{
		NativePtr->set_material(mtl->GetSmartPtr());
	}
}//namespace NexusEngine