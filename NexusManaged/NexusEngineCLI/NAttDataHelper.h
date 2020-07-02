#pragma once
#include "NWrapperException.h"

namespace NexusEngine
{
	using namespace NexusEngineExtension;

	// 模板成员属性声明宏
	// CLASS		: 数据类型名称，必须是值类型，并且是要和Native type一样字节大小的
	// PropName		: property的名称
	// NativeMember : 操作的native变量
	// TitleMapperName : Excel表中的列头名称，用于解析Excel表中该property对应哪个列
#define DECLARE_ATTINFO_PROPERTY(CLASS,PropName,NativeMember,TitleMapperName)\
	[Category("Info")]	\
	[TitleMapper(TitleMapperName)]	\
	property CLASS PropName\
	{\
		CLASS get()\
		{\
			return NativeMember;\
		}\
		void set(CLASS val)\
		{\
			NativeMember = val;\
		}\
	}

	// 模板成员属性声明宏
	// PropName			: property的名称
	// NativeMember		: 操作的native变量
	// TitleMapperName	: Excel表中的列头名称，用于解析Excel表中该property对应哪个列
#define DECLARE_ATTINFO_STRING_PROPERTY(PropName,NativeMember,TitleMapperName)\
	[Category("Info")]	\
	[TitleMapper(TitleMapperName)]	\
	property System::String^ PropName\
	{\
		System::String^ get()\
		{	return gcnew System::String(NativeMember.c_str()); }	\
		\
		void set(System::String^ val)\
		{\
			pin_ptr<const wchar_t> nativeStr = PtrToStringChars(val);	\
			NativeMember = nstring(nativeStr);\
		}\
	}

#define DECLARE_ATTINFO_NAME_PROPERTY(PropName,NativeMember,TitleMapperName)\
	[Category("Info")]	\
	[TitleMapper(TitleMapperName)]	\
	property System::String^ PropName	\
	{	\
		System::String^ get()	\
		{	return gcnew System::String( NativeMember.name_str.c_str() ); }\
		void set(System::String^ value)	\
		{	\
			pin_ptr<const wchar_t> nativeID = PtrToStringChars(value);	\
			NativeMember.reset(nativeID);	\
		}	\
	}	\
	property UInt32 Crc##PropName	\
	{	\
		UInt32 get()	\
		{	\
		return NativeMember.name_crc;	\
		}	\
	}	\


	// 声明宏，定义模板数据结构CLI封装
	// 为CLR提供方面Native struct访问接口
	// DECLARE_ATT_INFO_CLASS_BEGIN & DECLARE_ATT_INFO_CLASS_END必须成对出现 
#define DECLARE_ATT_INFO_CLASS_BEGIN(CLASS, NativeStruct, NativeInfoDataType)\
	public ref class CLASS	sealed	: public IDisposable\
	{	\
	private:	\
		boost::shared_ptr<NativeStruct>*  infoRef;	\
	public:	\
		explicit CLASS(boost::shared_ptr<NativeStruct> native_info)	\
		{	\
			infoRef = new boost::shared_ptr<NativeStruct>( native_info );\
		}	\
		explicit CLASS(System::String^ id)	\
		{	\
			pin_ptr<const wchar_t> nativeID = PtrToStringChars(id);	\
			boost::shared_ptr<NativeStruct> new_info( new nexus::NativeStruct(nativeID) );\
			gameframework::NativeInfoDataType::instance()->add_new_info(new_info);	\
			infoRef = new boost::shared_ptr<NativeStruct>( new_info );\
	}	\
		~CLASS(void)	\
		{	\
			this->!CLASS();\
		}\
		!CLASS(void)	\
		{	\
			if(infoRef)\
			{\
				infoRef->reset();\
				delete infoRef;\
				infoRef = NULL;\
			}\
		}\
		static System::Collections::Generic::Dictionary<UInt32, CLASS^>^ ConvertNativeInfos()\
		{\
			System::Collections::Generic::Dictionary<UInt32, CLASS^>^ rets\
				=  gcnew System::Collections::Generic::Dictionary<UInt32, CLASS^>();\
			const gameframework::NativeInfoDataType::info_container& native_infos \
				= gameframework::NativeInfoDataType::instance()->get_infos();\
			BOOST_FOREACH(gameframework::NativeInfoDataType::info_container::value_type i, native_infos)\
			{\
				rets->Add(i.first, gcnew CLASS(i.second));\
			}\
			return rets;\
		}\
		static void ClearInfo()\
		{\
			gameframework::NativeInfoDataType::instance()->clear_info();\
		}\
		static bool RemoveInfo(System::String^ strID)\
		{\
			pin_ptr<const wchar_t> nativeID = PtrToStringChars(strID);	\
			nname name(nativeID);\
			return gameframework::NativeInfoDataType::instance()->remove_info(name);\
		}\
		static bool RemoveInfo(UInt32 crcID)\
		{\
			return gameframework::NativeInfoDataType::instance()->remove_info(crcID);\
		}\
		static CLASS^ FindInfo(UInt32 crcID)\
		{\
			boost::shared_ptr<NativeStruct> native_obj = gameframework::NativeInfoDataType::instance()->find_info(crcID);\
			if(native_obj)\
			{\
				return gcnew CLASS(native_obj);\
			}\
			return nullptr;\
		}\
		static CLASS^ FindInfo(System::String^ strID)\
		{\
			pin_ptr<const wchar_t> nativeID = PtrToStringChars(strID);	\
			nname name(nativeID);\
			boost::shared_ptr<NativeStruct> native_obj = gameframework::NativeInfoDataType::instance()->find_info(name);\
			if(native_obj)\
			{\
				return gcnew CLASS(native_obj);\
			}\
			return nullptr;\
		}\
		/** 从文件中加载 */\
		static bool LoadFromFile()\
		{\
			wostringstream oss;\
			oss << _T("attribute/")\
				<< #NativeInfoDataType\
				<< _T(".att");\
			BEGIN_NATIVE_GUARD\
			return gameframework::NativeInfoDataType::instance()->serialize(\
				NEngine::Instance()->FileSystem->NativePtr,\
				_T("content"),\
				oss.str(),\
				EFileRead );\
			END_NATIVE_GUARD\
		}\
		\
		/**  将当前资源保存到文件 */\
		static bool SaveToFile()\
		{\
			wostringstream oss;\
			oss << _T("attribute/")\
				<< #NativeInfoDataType\
				<< _T(".att");\
			BEGIN_NATIVE_GUARD\
			return gameframework::NativeInfoDataType::instance()->serialize(\
				NEngine::Instance()->FileSystem->NativePtr,\
				_T("content"),\
				oss.str(),\
				EFileWrite );\
			END_NATIVE_GUARD\
		}\
		/** 对象访问 */	\
		property nexus::NativeStruct* NativePtr	\
		{	\
			nexus::NativeStruct* get()	\
			{	\
				if(infoRef)\
				{\
					return infoRef->get();\
				}\
				else\
				{\
					return NULL;\
				}\
			}	\
		}	\
		property boost::shared_ptr<NativeStruct>* SharedPtr\
		{	\
			boost::shared_ptr<NativeStruct>* get()	\
			{	\
				return infoRef;	\
			}	\
		}	\
		/** 属性访问 */\
		[Category("Info")]	\
		[TitleMapper("ID")]	\
		property System::String^ ID	\
		{	\
			System::String^ get()	\
			{	return gcnew System::String( NativePtr->id.name_str.c_str() ); }\
			void set(System::String^ value)	\
			{	\
				pin_ptr<const wchar_t> nativeID = PtrToStringChars(value);	\
				NativePtr->id.reset(nativeID);	\
			}	\
		}	\
		property UInt32 CrcID	\
		{	\
			UInt32 get()	\
			{	\
				return NativePtr->id.name_crc;	\
			}	\
		}	\
		DECLARE_ATTINFO_STRING_PROPERTY(Name,NativePtr->name,"名称");	\
		DECLARE_ATTINFO_STRING_PROPERTY(Description,NativePtr->description,"描述");


		// 结束定义模板数据结构CLI封装
#define DECLARE_ATT_INFO_CLASS_END()\
	};	\

} // end of namespace NexusEngine