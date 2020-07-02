#pragma once
#include "NEngine.h"
#include "NFileSystem.h"
#include "NAttDataHelper.h"
#include "ndef_levelup.h"
#include "ngameframework.h"
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

namespace NexusEngine
{
	using namespace NexusEngineExtension;
	using namespace nexus;
	using namespace gameframework;

	/**
	* class NLevelupInfo
	* 对nexus::levelup_info封装
	* 为CLR提供方面Native struct接口
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NLevelupInfo, levelup_info, nlevelup_info_dictionary);
		// 经验
		DECLARE_ATTINFO_PROPERTY(UInt32,Exp,NativePtr->exp,"经验");

	DECLARE_ATT_INFO_CLASS_END();

	//public ref class NLevelupInfo	sealed : public IDisposable
	//{	
	//private:	
	//	typedef boost::weak_ptr<nexus::levelup_info> levelup_info_weak_ptr;
	//	levelup_info_weak_ptr*  infoRef;	
	//public:	
	//	explicit NLevelupInfo(nexus::levelup_info_ptr native_info)	
	//	{	
	//		infoRef = new levelup_info_weak_ptr( native_info );
	//	}	
	//	explicit NLevelupInfo(System::String^ id)	
	//	{	
	//		// 方便在导入资源的时候能够通过反射构造对象
	//		pin_ptr<const wchar_t> nativeID = PtrToStringChars(id);	
	//		levelup_info_ptr new_info( new nexus::levelup_info(nativeID) );
	//		gameframework::nlevelup_info_data::instance()->add_new_info(new_info);	
	//		infoRef = new levelup_info_weak_ptr( new_info );
	//	}	
	//	~NLevelupInfo(void)	
	//	{	
	//		this->!NLevelupInfo();
	//	}
	//	!NLevelupInfo(void)	
	//	{	
	//		infoRef->reset();
	//		delete infoRef;
	//		infoRef = NULL;
	//	}
	//	/** Converts the value of the object to its equivalent string representation. */
	//	virtual System::String^ ToString() override
	//	{
	//		return ID + "-" + Name + "-" + Description;
	//	}
	//	static System::Collections::Generic::Dictionary<UInt32, NLevelupInfo^>^ ConvertNativeInfos()
	//	{
	//		System::Collections::Generic::Dictionary<UInt32, NLevelupInfo^>^ rets
	//			=  gcnew System::Collections::Generic::Dictionary<UInt32, NLevelupInfo^>();
	//		const gameframework::nlevelup_info_data::info_container& native_infos 
	//			= gameframework::nlevelup_info_data::instance()->get_infos();
	//		BOOST_FOREACH(gameframework::nlevelup_info_data::info_container::value_type i, native_infos)
	//		{
	//			rets->Add(i.first, gcnew NLevelupInfo(i.second));
	//		}
	//		return rets;
	//	}
	//	static void ClearInfo()
	//	{
	//		gameframework::nlevelup_info_data::instance()->clear_info();
	//	}
	//	static bool RemoveInfo(System::String^ strID)
	//	{
	//		pin_ptr<const wchar_t> nativeID = PtrToStringChars(strID);	
	//		nname name(nativeID);
	//		return gameframework::nlevelup_info_data::instance()->remove_info(name);
	//	}
	//	static bool RemoveInfo(UInt32 crcID)
	//	{
	//		return gameframework::nlevelup_info_data::instance()->remove_info(crcID);
	//	}
	//	static NLevelupInfo^ FindInfo(UInt32 crcID)
	//	{
	//		levelup_info_ptr native_obj = gameframework::nlevelup_info_data::instance()->find_info(crcID);
	//		if(native_obj)
	//		{
	//			return gcnew NLevelupInfo(native_obj);
	//		}
	//		return nullptr;
	//	}
	//	static NLevelupInfo^ FindInfo(System::String^ strID)
	//	{
	//		pin_ptr<const wchar_t> nativeID = PtrToStringChars(strID);	
	//		nname name(nativeID);
	//		levelup_info_ptr native_obj = gameframework::nlevelup_info_data::instance()->find_info(name);
	//		if(native_obj)
	//		{
	//			return gcnew NLevelupInfo(native_obj);
	//		}
	//		return nullptr;
	//	}
	//	/** 从文件中加载 */
	//	static bool LoadFromFile()
	//	{
	//		return gameframework::nlevelup_info_data::instance()->serialize(
	//			NEngine::Instance()->FileSystem->NativePtr,
	//			_T("content"),
	//			_T("attribute/nlevelup_info_data.att"),
	//			EFileRead );
	//	}
	//	/**  将当前资源保存到文件 */
	//	static bool SaveToFile()
	//	{
	//		return gameframework::nlevelup_info_data::instance()->serialize(
	//			NEngine::Instance()->FileSystem->NativePtr,
	//			_T("content"),
	//			_T("attribute/nlevelup_info_data.att"),
	//			EFileWrite );
	//	}
	//	/** 对象访问 */
	//	property nexus::levelup_info* NativePtr
	//	{
	//		nexus::levelup_info* get()
	//		{
	//			levelup_info_ptr new_info(*infoRef);
	//			if(new_info)
	//			{
	//				return new_info.get();
	//			}
	//			else
	//			{
	//				return NULL;
	//			}
	//		}
	//	}
	//	property levelup_info_weak_ptr* SharedPtr
	//	{
	//		levelup_info_weak_ptr* get()
	//		{
	//			return infoRef;
	//		}
	//	}
	//	/** 属性访问 */
	//	[Category("Info")]	
	//	[TitleMapper("ID")]	
	//	property System::String^ ID	
	//	{	
	//		System::String^ get()	
	//		{	return gcnew System::String( NativePtr->id.name_str.c_str() ); }	
	//		void set(System::String^ value)	
	//		{	
	//			pin_ptr<const wchar_t> nativeID = PtrToStringChars(value);	
	//			NativePtr->id.reset(nativeID);	
	//		}	
	//	}	
	//	property UInt32 CrcID	
	//	{	
	//		UInt32 get()	
	//		{	
	//			return NativePtr->id.name_crc;	
	//		}	
	//	}	
	//	DECLARE_ATTINFO_STRING_PROPERTY(Name,NativePtr->name,"名称");	
	//	DECLARE_ATTINFO_STRING_PROPERTY(FriendName,NativePtr->friend_name,"FriendName");	

	//	DECLARE_ATTINFO_STRING_PROPERTY(Description,NativePtr->description,"描述");	

	//	DECLARE_ATTINFO_PROPERTY(UInt32,Exp,NativePtr->exp,"经验");

	//};
}
