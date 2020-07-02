#include "StdAfx.h"
#include "NActor.h"
#include "NTerrainActor.h"
#include "NWrapperException.h"
#include "NActorComponent.h"
#include "NLevel.h"

using namespace System::Collections::Generic;

namespace NexusEngine
{
	NActor::NActor(nexus::nactor::ptr nativeActor):NativeObjectOwnedBySharedPtr(nativeActor)
	{
		m_compList = gcnew List<NActorComponent^>;
	}
	
	nactor* NActor::NativePtr::get()
	{
		return dynamic_cast<nactor*>(RawPtr);
	}

	NActor^ NActor::FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName)
	{
		if( nativeClassName == L"nactor" )
			return gcnew NActor(nativeActor);
		else if( nativeClassName == L"nterrain_actor" )
		{
			nexus::nterrain_actor::ptr nativeTrn = boost::dynamic_pointer_cast<nterrain_actor>(nativeActor);
			return gcnew NTerrainActor(nativeTrn);
		}
		else
		{
			throw gcnew NexusNativeException(nativeClassName+L" NO managed wrapper class.");
		}

		return nullptr;
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

	void NActor::Name::set(System::String^ val)
	{
		String^ oldName = gcnew System::String(NativePtr->get_name().name_str.c_str());
		m_owner->RenameActor(oldName, val);		
	}
}//namespace NexusEngine