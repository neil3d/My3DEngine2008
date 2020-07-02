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
		m_RenderSetting=gcnew RenderSetting(NativePtr->get_render_setting());
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
		if(NativePtr==nullptr)
		{
			return gcnew System::String( _T("") );
		}
		return gcnew System::String( NativePtr->get_name().c_str() );
	}

	void NLevel::Init()
	{
		NativePtr->init();
	}

	NActor^ NLevel::CreateActor(System::String^ _actorName, System::String^ nativeClassName)
	{
		int i=1;
		System::String^ actorName=_actorName;
		while(m_actorDict->ContainsKey(actorName))
		{
			actorName=_actorName+L"_"+Convert::ToString(i);
			++i;
		}
		pin_ptr<const wchar_t> szActorName = PtrToStringChars(actorName);
		pin_ptr<const wchar_t> szNativeClassName= PtrToStringChars(nativeClassName);

		nexus::nactor::ptr newActor = NativePtr->create_actor(szActorName, szNativeClassName);
		if(newActor==NULL)
		{
			return nullptr;
		}
		NActor^ mActor = NActor::FromNativePtr(newActor, nativeClassName);
		mActor->_SetOwner(this);
		m_actorDict->Add(actorName, mActor);

		return mActor;
	}

	void NLevel::DestroyActor(System::String^ actorName)
	{
		NActor^ targetActor = FindActor(actorName);
		if(targetActor == nullptr)
			throw gcnew System::ArgumentException("Actor Not Found.");

		pin_ptr<const wchar_t> szActorName = PtrToStringChars(actorName);
		nexus::nname nameObj(szActorName);
		if(NativePtr->destroy_actor( nameObj ))
		{
			targetActor->Destroyed = true;
			m_actorDict->Remove( actorName );
		}
	}

	void NLevel::DestroyAllActors()
	{
		m_actorDict->Clear();
		NativePtr->destroy_all_actors();
	}

	NActor^ NLevel::FindActor(System::String^ actorName)
	{
		// Dictionary索引器会回抛出异常，正确的做法是之前使用 ContainsKey 方法测试某个键是否存在
		// 请参考MSDN
		if(m_actorDict->ContainsKey(actorName))
		{
			return m_actorDict[actorName];
		}
		return nullptr;
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
			mActor->_SetOwner(this);
			m_actorDict->Add(actorName, mActor);

			na = NativePtr->next_actor();
		}
	}

	bool NLevel::LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end, LineCheckType checkType)
	{
		ncheck_result nret;
		nexus::vector3* nstart = reinterpret_cast<nexus::vector3*>(&start);
		nexus::vector3* nend = reinterpret_cast<nexus::vector3*>(&end);

		if( NativePtr->line_check(nret, *nstart, *nend,(ELineCheckType)checkType) )
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


	bool NLevel::LineOfSight( [Out] NCheckResult% ret, Vector3 start, Vector3 end, bool checkWalkableMap )
	{
		ncheck_result nret;
		nexus::vector3* nstart = reinterpret_cast<nexus::vector3*>(&start);
		nexus::vector3* nend = reinterpret_cast<nexus::vector3*>(&end);

		if( NativePtr->line_of_sight(nret, *nstart, *nend,checkWalkableMap) )
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

		// if(native code rename actor) then (managed code rename actor)
		if(NativePtr->rename_actor(szOldName, szNewName))
		{
			NActor^ findActor = m_actorDict[oldName];
			m_actorDict->Remove(oldName);

			System::Diagnostics::Debug::Assert(findActor->Name == newName);

			m_actorDict->Add(newName, findActor);
		}
	}

	void NLevel::AttachActor(NActor^ actor)
	{
		if(m_actorDict->ContainsKey(actor->Name))
		{
			throw gcnew System::ArgumentException("Actor Already Exits.");
		}

		NativePtr->attach_actor( actor->SharedPtr );

		actor->Destroyed = false;
		actor->_SetOwner(this);
		m_actorDict->Add( actor->Name, actor);
	}

	bool NLevel::DeattachActor(NActor^ actor)
	{
		if(!m_actorDict->ContainsKey(actor->Name))
			return false;

		bool ret = NativePtr->deattach_actor( actor->SharedPtr );

		actor->Destroyed = true;
		actor->_SetOwner(nullptr);
		m_actorDict->Remove( actor->Name );

		return ret;
	}

	void NLevel::CreateNewLayer( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		NativePtr->new_layer(layer_name);
	}

	void NLevel::RemoveLayer( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		NativePtr->remove_layer(layer_name);
	}

	void NLevel::RenameLayer( String^ oldLayerName,String^ NewlayerName )
	{
		pin_ptr<const wchar_t> szOldName = PtrToStringChars(oldLayerName);
		pin_ptr<const wchar_t> szNewName = PtrToStringChars(NewlayerName);
		NativePtr->rename_layer(szOldName,szNewName);
	}

	void NLevel::SetActiveLayer( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		NativePtr->set_active_layer(layer_name);
	}

	String^ NLevel::GetActiveLayer()
	{
		return gcnew System::String( NativePtr->get_active_layer().c_str() );
	}

	void NLevel::LockLayer( String^ layerName,bool lock )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		NativePtr->lock_layer(layer_name,lock);
	}

	bool NLevel::IsLayerLocked( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		return NativePtr->is_layer_locked(layer_name);		
	}

	void NLevel::ShowLayer( String^ layerName,bool show )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		NativePtr->show_layer(layer_name,show);
	}

	bool NLevel::IsLayerVisible( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		return NativePtr->is_layer_visible(layer_name);		 
	}

	bool NLevel::IsLayerExist( String^ layerName )
	{
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(layerName);
		return NativePtr->is_layer_exist(layer_name);	
	}

	int NLevel::GetLayerCount()
	{
		return static_cast<int>(NativePtr->get_layer_count());
	}

	String^ NLevel::GetLayerName( int index )
	{
		return gcnew System::String( NativePtr->get_layer_name(index).c_str() );
	}

	void NLevel::MoveActorToActiveLayer( String^ actorName )
	{
		pin_ptr<const wchar_t> actor_name = PtrToStringChars(actorName);
		NativePtr->move_actor_to_active_layer(actor_name);	
	}

	bool NLevel::GetWorldWalkable( float wx, float wz )
	{
		return NativePtr->get_world_walkable(wx,wz);
	}

	bool NLevel::GetTileWalkable( int tx, int tz )
	{
		return NativePtr->get_tile_walkable(tx,tz);
	}

	float NLevel::GetWorldHeight( float wx, float wz )
	{
		return NativePtr->get_world_height(wx,wz);
	}

	float NLevel::GetTileHeight( int tx, int tz )
	{
		return NativePtr->get_tile_height(tx,tz);
	}

	NActor^ NLevel::CloneActor( NActor^ actor )
	{
		nexus::nactor::ptr newActor = NativePtr->clone_actor(actor->SharedPtr);
		if(newActor==NULL)
		{
			return nullptr;
		}
		NActor^ mActor = actor->Clone(newActor);
		mActor->_SetOwner(this);
		m_actorDict->Add(mActor->Name, mActor);

		return mActor;
	}

	int NLevel::ExportLevel( NResourceLoc^ location )
	{
		nexus::resource_location native_loc;
		location->ToNative(native_loc);

		return NativePtr->export_level(native_loc);
	}
}//namespace NexusEngine