#include "StdAfx.h"
#include "NPrimitiveComponent.h"

namespace NexusEngine
{
	NPrimitiveComponent::NPrimitiveComponent(nexus::nprimitive_component::ptr nativeComp):NActorComponent(nativeComp)
	{
	}

	nprimitive_component* NPrimitiveComponent::NativePtr::get()
	{
		return dynamic_cast<nprimitive_component*>(RawPtr);
	}

	BoundingBox NPrimitiveComponent::GetBoundingBox()
	{
		nexus::AABBox nbox = NativePtr->get_AABBox();
		
		BoundingBox mbox;
		mbox.Maximum = Vector3::FromNative(nbox.m_max);
		mbox.Minimum = Vector3::FromNative(nbox.m_min);

		return mbox;
	}
}//namespace NexusEngine