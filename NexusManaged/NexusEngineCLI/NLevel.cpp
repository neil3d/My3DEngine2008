#include "StdAfx.h"
#include "NLevel.h"
#include "NWrapperException.h"

using namespace System;
using namespace System::Collections::Generic;

namespace NexusEngine
{
	NLevel::NLevel(nexus::nlevel::ptr nativeLv):NativeObjectOwnedBySharedPtr(nativeLv)
	{
		m_actorDict = gcnew Dictionary<String^, NActor^>();
	}

	NLevel^ NLevel::FromNativePtr(nexus::nlevel::ptr nativeLv, System::String^ nativeClassName)
	{
		if(nativeClassName == L"nlevel")
			return gcnew NLevel(nativeLv);
		else
		{
			throw gcnew NexusNativeException(nativeClassName+L" NO managed wrapper class.");
		}
		return nullptr;
	}

	nexus::nlevel* NLevel::NativePtr::get()
	{
		return dynamic_cast<nlevel*>(RawPtr);
	}

	nexus::nlevel::ptr NLevel::GetNativeSmartPtr()
	{
		return boost::dynamic_pointer_cast<nexus::nlevel>(*m_smart_ptr);
	}

	System::String^ NLevel::Name::get()
	{
		return gcnew System::String( NativePtr->get_name().c_str() );
	}

	void NLevel::Init(System::String^ sceneClassName)
	{
		pin_ptr<const wchar_t> szClassName = PtrToStringChars(sceneClassName);
		NativePtr->init(szClassName);
	}

	NActor^ NLevel::CreateActor(System::String^ actorName, System::String^ nativeClassName)
	{
		pin_ptr<const wchar_t> szActorName = PtrToStringChars(actorName);
		pin_ptr<const wchar_t> szNativeClassName= PtrToStringChars(nativeClassName);

		nexus::nactor::ptr newActor = NativePtr->create_actor(szActorName, szNativeClassName);
		NActor^ mActor = NActor::FromNativePtr(newActor, nativeClassName);
		mActor->_SetOwner(this);
		m_actorDict->Add(actorName, mActor);

		return mActor;
	}

	void NLevel::DestroyActor(System::String^ actorName)
	{
		pin_ptr<const wchar_t> szActorName = PtrToStringChars(actorName);
		nexus::nname nameObj(szActorName);
		NativePtr->destroy_actor( nameObj );
		m_actorDict->Remove( actorName );
	}

	void NLevel::DestroyAllActors()
	{
		m_actorDict->Clear();
		NativePtr->destroy_all_actors();
	}

	NActor^ NLevel::FindActor(System::String^ actorName)
	{
		return m_actorDict[actorName];
	}

	void NLevel::NativeLevelLoaded()
	{
		nactor::ptr na = NativePtr->first_actor();
		while( na )
		{
			System::String^ nativeClassName = gcnew System::String(na->reflection_get_class()->get_name());
			System::String^ actorName = gcnew System::String(na->get_name_str().c_str());

			NActor^ mActor = NActor::FromNativePtr(na, nativeClassName);
			mActor->NativeActorLoaded();
			m_actorDict->Add(actorName, mActor);

			na = NativePtr->next_actor();
		}
	}

	bool NLevel::LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end)
	{
		ncheck_result nret;
		nexus::vector3* nstart = reinterpret_cast<nexus::vector3*>(&start);
		nexus::vector3* nend = reinterpret_cast<nexus::vector3*>(&end);

		if( NativePtr->line_check(nret, *nstart, *nend) )
		{
			ret.location.x = nret.location.x;
			ret.location.y = nret.location.y;
			ret.location.z = nret.location.z;
			
			ret.normal.x = nret.normal.x;
			ret.normal.y = nret.normal.y;
			ret.normal.z = nret.normal.z;
			
			ret.dist = nret.dist;
			return true;
		}		

		return false;
	}

	NActor^ NLevel::FirstActor()
	{
		m_actorEnum = m_actorDict->GetEnumerator();		
		return NextActor();
	}

	NActor^ NLevel::NextActor()
	{
		if(m_actorEnum->MoveNext())
		{
			KeyValuePair<String^, NActor^>^ item = m_actorEnum->Current;
			return item->Value;
		}
		else
			return nullptr;
	}

	void NLevel::RenameActor(String^ oldName, String^ newName)
	{
		if( !m_actorDict->ContainsKey(oldName) )
			throw gcnew System::ArgumentException("Actor Not Found.");

		pin_ptr<const wchar_t> szOldName = PtrToStringChars(oldName);
		pin_ptr<const wchar_t> szNewName = PtrToStringChars(newName);

		NActor^ findActor = m_actorDict[oldName];
		m_actorDict->Remove(oldName);

		NativePtr->rename_actor(szOldName, szNewName);

		System::Diagnostics::Debug::Assert(findActor->Name == newName);

		m_actorDict->Add(newName, findActor);
	}
}//namespace NexusEngine