#include "StdAfx.h"
#include "NActorComponent.h"
#include "NStaticMeshComponent.h"
#include "NSpeedTreeComponent.h"
#include "NWrapperException.h"
#include "NQuadTreeTerrain.h"
#include "NLightComponent.h"
#include "NShapeComponent.h"
#include "NAnimMeshComponent.h"

namespace NexusEngine
{
	NActorComponent::NActorComponent(nexus::nactor_component::ptr nativeComp):NativeObjectOwnedBySharedPtr(nativeComp)
	{
	}

	void NActorComponent::_SetOwner(NActor^ owner)
	{
		m_owner = gcnew System::WeakReference(owner);
	}

	nactor_component* NActorComponent::NativePtr::get()
	{
		return dynamic_cast<nactor_component*>(RawPtr);
	}

	NActor^ NActorComponent::GetOwner()
	{
		if( m_owner->IsAlive )
		{
			return reinterpret_cast<NActor^>(m_owner->Target);
		}
		return nullptr;
	}

	NActorComponent^ NActorComponent::FromNativePtr(nexus::nactor_component::ptr nativeComp, System::String^ nativeClassName)
	{
		
		if(nativeClassName==L"nstatic_mesh_component")
		{
			boost::shared_ptr<nexus::nstatic_mesh_component> nativeMesh = 
				boost::dynamic_pointer_cast<nexus::nstatic_mesh_component>(nativeComp);
			return gcnew NStaticMeshComponent(nativeMesh);
		}
		else if(nativeClassName==L"nanim_mesh_component")
		{
			boost::shared_ptr<nexus::nanim_mesh_component> nativeMesh = 
				boost::dynamic_pointer_cast<nexus::nanim_mesh_component>(nativeComp);
			return gcnew NAnimMeshComponent(nativeMesh);
		}
		else if(nativeClassName==L"nspeed_tree_component")
		{
			boost::shared_ptr<nexus::nspeed_tree_component> nativeSPT = 
				boost::dynamic_pointer_cast<nexus::nspeed_tree_component>(nativeComp);
			return gcnew NSpeedTreeComponent(nativeSPT);
		}
		else if(nativeClassName==L"nquad_tree_terrain")
		{
			boost::shared_ptr<nexus::nprimitive_component> nativeTrn = 
				boost::dynamic_pointer_cast<nexus::nprimitive_component>(nativeComp);
			return gcnew NQuadTreeTerrain(nativeTrn);
		}
		else if(nativeClassName==L"nlight_component")
		{
			boost::shared_ptr<nexus::nlight_component> nativeLgt = 
				boost::dynamic_pointer_cast<nexus::nlight_component>(nativeComp);
			return gcnew NLightComponent(nativeLgt);
		}
		else if(nativeClassName==L"nshape_component")
		{
			boost::shared_ptr<nexus::nshape_component> nativeShape = 
				boost::dynamic_pointer_cast<nexus::nshape_component>(nativeComp);
			return gcnew NShapeComponent(nativeShape);
		}
		else
		{
			throw gcnew NexusNativeException(nativeClassName+L" NO managed wrapper class.");
		}

		return nullptr;
	}

	System::String^ NActorComponent::Name::get()
	{
		return gcnew System::String(NativePtr->get_name().name_str.c_str());
	}

	System::String^ NActorComponent::NativeClassName::get()
	{
		const TCHAR* nclassName = NativePtr->reflection_get_class()->get_name();
		return gcnew System::String(nclassName);
	}
}//namespace NexusEngine