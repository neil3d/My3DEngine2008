#include "StdAfx.h"
#include "NActor.h"
#include "NTerrainActor.h"
#include "NWrapperException.h"
#include "NActorComponent.h"
#include "NLevel.h"
#include "NTriggerActor.h"
#include "NStartPointActor.h"
#include "NAvatarActor.h"

using namespace System::Collections::Generic;

namespace NexusEngine
{
	NActor::NActor(nexus::nactor::ptr nativeActor):NativeObjectOwnedBySharedPtr(nativeActor)
	{
		m_compList = gcnew List<NActorComponent^>;
		m_destroyed = false;
	}
	
	nactor* NActor::NativePtr::get()
	{
		return dynamic_cast<nactor*>(RawPtr);
	}

	nactor::ptr NActor::SharedPtr::get()
	{
		return boost::shared_dynamic_cast<nactor>(*m_smart_ptr);
	}

	NActor^ NActor::FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName)
	{
		if( nativeClassName == L"nterrain_actor" )
		{
			nexus::nterrain_actor::ptr nativeTrn = boost::dynamic_pointer_cast<nterrain_actor>(nativeActor);
			return gcnew NTerrainActor(nativeTrn);
		}
		else if( nativeClassName == L"ntrigger_actor" )
		{
			nexus::ntrigger_actor::ptr nativePtr = boost::dynamic_pointer_cast<ntrigger_actor>(nativeActor);
			return gcnew NTriggerActor(nativePtr);
		}
		else if( nativeClassName == L"nstart_point_actor" )
		{
			nexus::nstart_point_actor::ptr nativePtr = boost::dynamic_pointer_cast<nstart_point_actor>(nativeActor);
			return gcnew NStartPointActor(nativePtr);
		}
		else if( nativeClassName == L"navatar_actor" )
		{
			nexus::navatar_actor::ptr nativePtr = boost::dynamic_pointer_cast<navatar_actor>(nativeActor);
			return gcnew NAvatarActor(nativePtr);
		}

		// 如果没有没有managed封装的一律创建缺省的NActor
		return gcnew NActor(nativeActor);
	}

	NActorComponent^ NActor::CreateComponent(System::String^ compName, System::String^ nativeClassName)
	{
		pin_ptr<const wchar_t> szCompName = PtrToStringChars(compName);
		pin_ptr<const wchar_t> szNativeClassName = PtrToStringChars(nativeClassName);
		nexus::nactor_component::ptr nativeComp = NativePtr->create_component(szCompName, szNativeClassName);

		NActorComponent^ newComp = NActorComponent::FromNativePtr(nativeComp, nativeClassName);

		newComp->_SetOwner(this);
		m_compList->Add(newComp);

		return newComp;
	}

	void NActor::DrawBounds(NRenderPrimitiveDrawInterface^ PDI, Color4f color)
	{
		nexus::color4f* nc = reinterpret_cast<nexus::color4f*>(&color);
		NativePtr->draw_bounds(PDI->NativePtr, *nc);
	}

	void NActor::NativeActorLoaded()
	{
		if( NativePtr->get_num_components() <= 0 )
			return;

		nexus::nactor_component::ptr nativeComp = NativePtr->first_component();
		while( nativeComp )
		{
			const TCHAR* nativeClassName = nativeComp->reflection_get_class()->get_name();
			NActorComponent^ newComp = NActorComponent::FromNativePtr(nativeComp, 
				gcnew System::String(nativeClassName) );

			newComp->_SetOwner(this);
			m_compList->Add(newComp);

			nativeComp = NativePtr->next_component();
		}

	}

	bool NActor::IsSameTypeOf( NActor^ actor )
	{
		nactor* a=NativePtr;
		nactor* b=actor->NativePtr;
		return a->has_same_type_component(b);
	}

	NActor^ NActor::Clone( nexus::nactor::ptr nativeActor )
	{
		String^ class_name=gcnew String(nativeActor->reflection_get_class()->get_name());
		NActor^ actor= FromNativePtr(nativeActor,class_name);
		actor->NativeActorLoaded();
		return actor;
	}

	void NActor::Name::set(System::String^ val)
	{
		String^ oldName = gcnew System::String(NativePtr->get_name().name_str.c_str());
		m_owner->RenameActor(oldName, val);		
	}

	void NActor::Layer::set(System::String^ val)
	{
		if(!m_owner->IsLayerExist(val))
		{
			val=m_owner->GetActiveLayer();
		}
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(val);
		NativePtr->set_layer(layer_name);
	}

	ObjectSpace NActor::SpaceData::get()
	{
		const object_space& os = NativePtr->get_space();
		
		ObjectSpace ret;
		ret.location = Vector3::FromNative(os.location);
		ret.rotation = Vector3::FromNative(os.rotation);
		ret.scale = Vector3::FromNative(os.scale);

		return ret;
	}

	void NActor::SpaceData::set(ObjectSpace newSpace)
	{
		vector3 nloc = newSpace.location.ToNative();
		vector3 nrot = newSpace.rotation.ToNative();
		vector3 nscale = newSpace.scale.ToNative();
		
		NativePtr->move(nloc, nrot, nscale);
	}
}//namespace NexusEngine