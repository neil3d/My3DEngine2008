#include "StdAfx.h"
#include "NLog.h"
#include "NActorComponent.h"
#include "NStaticMeshComponent.h"
#include "NSpeedTreeComponent.h"
#include "NWrapperException.h"
#include "NLightComponent.h"
#include "NFogComponent.h"
#include "NShapeComponent.h"
#include "NAnimMeshComponent.h"
#include "NSpecialEffectInstance.h"
#include "NSkeletalMeshComponent.h"
#include "NSkyComponent.h"
#include "NWaterComponent.h"

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
		else if(nativeClassName==L"nstatic_editor_mesh_component")
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
		else if (nativeClassName==L"npoint_light_component")
		{
			boost::shared_ptr<nexus::npoint_light_component> nativeLgt = 
				boost::dynamic_pointer_cast<nexus::npoint_light_component>(nativeComp);
			return gcnew NPointLightComponent(nativeLgt);
		}
		else if (nativeClassName==L"ndirectional_light_component")
		{
			boost::shared_ptr<nexus::ndirectional_light_component> nativeLgt = 
				boost::dynamic_pointer_cast<nexus::ndirectional_light_component>(nativeComp);
			return gcnew NDirectionalLightComponent(nativeLgt);
		}
		else if (nativeClassName==L"nspot_light_component")
		{
			boost::shared_ptr<nexus::nspot_light_component> nativeLgt = 
				boost::dynamic_pointer_cast<nexus::nspot_light_component>(nativeComp);
			return gcnew NSpotLightComponent(nativeLgt);
		}
		else if (nativeClassName==L"nfog_volume_component")
		{
			boost::shared_ptr<nexus::nfog_volume_component> nativeLgt = 
				boost::dynamic_pointer_cast<nexus::nfog_volume_component>(nativeComp);
			return gcnew NFogComponent(nativeLgt);
		}
		else if (nativeClassName==L"nwater_component")
		{
			boost::shared_ptr<nexus::nwater_component> nativeWater = 
				boost::dynamic_pointer_cast<nexus::nwater_component>(nativeComp);
			return gcnew NWaterComponent(nativeWater);
		}
		else if(nativeClassName==L"nshape_component")
		{
			boost::shared_ptr<nexus::nshape_component> nativeShape = 
				boost::dynamic_pointer_cast<nexus::nshape_component>(nativeComp);
			return gcnew NShapeComponent(nativeShape);
		}
		else if(nativeClassName==L"nspecial_effect_instance")
		{
			boost::shared_ptr<nexus::nspecial_effect_instance> nativeSFX = 
				boost::dynamic_pointer_cast<nexus::nspecial_effect_instance>(nativeComp);
			return gcnew NSpecialEffectInstance(nativeSFX);
		}
		else if(nativeClassName==L"nskeletal_mesh_component")
		{
			boost::shared_ptr<nexus::nskeletal_mesh_component> nativeSMC = 
				boost::dynamic_pointer_cast<nexus::nskeletal_mesh_component>(nativeComp);
			return gcnew NSkeletalMeshComponent(nativeSMC);
		}
		else if(nativeClassName==L"nsky_component")
		{
			boost::shared_ptr<nexus::nsky_component> nativeSMC = 
				boost::dynamic_pointer_cast<nexus::nsky_component>(nativeComp);
			return gcnew NSkyComponent(nativeSMC);
		}
		else
		{
			NLogger::Instance->WriteString(LogType::Warning, String::Format("Has not CLI wraper class {0}",nativeClassName));
			// 如果没有特定的包装类，则统一返回基类包装类对象，只提供基类接口
			return gcnew NActorComponent(nativeComp);
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

	bool NActorComponent::Editable::get()
	{
		return NativePtr->get_editable();
	}
}//namespace NexusEngine