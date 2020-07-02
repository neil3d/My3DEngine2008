#include "StdAfx.h"
#include "NEngine.h"
#include "NFileSystem.h"
#include "NRenderElement.h"
#include "NWrapperException.h"
#include "NRenderResourceManager.h"

using namespace System;
using namespace System::Collections::Generic;

namespace NexusEngine
{
	void NEngineConfig::ToNative(engine_config& cfg)
	{
		cfg.window_handle	= RenderWndHandle.ToPointer();
		cfg.width			= ClientWidth;
		cfg.height			= ClientHeight;
		cfg.color_bits		= ColorBits;
		cfg.bWindowed		= !FullScreen;
		cfg.bEnableHDR		= EnableHDR;
		cfg.bEnableSSAO = EnableSSAO;

		pin_ptr<const wchar_t> szRenderClass = PtrToStringChars(RenderClass);
		pin_ptr<const wchar_t> szFileSystemClass = PtrToStringChars(FileSystemClass);
		pin_ptr<const wchar_t> szFileSystemRoot = PtrToStringChars(FileSystemRoot);
		pin_ptr<const wchar_t> szEngineDataPkg = PtrToStringChars(EngineDataPkg);
		pin_ptr<const wchar_t> szResourceCacheClass = PtrToStringChars(ResourceCacheClass);
		pin_ptr<const wchar_t> szResourceIOClass = PtrToStringChars(ResourceIOClass);

		cfg.renderer_class		= szRenderClass;
		cfg.file_system_class	= szFileSystemClass;
		cfg.file_sys_root		= szFileSystemRoot;
		cfg.engine_data_pkg		= szEngineDataPkg;
		cfg.resource_cache_class = szResourceCacheClass;
		cfg.resource_io_class	= szResourceIOClass;
	}

	NEngine::NEngine(void):NativeObjectOwnedRaw(new nengine)
	{
		s_instance = this;		
	}

	NEngine::NEngine(System::String^ nativeClassName):NativeObjectOwnedRaw(INativeObj::ConstructObject(nativeClassName))
	{
		s_instance = this;		
	}

	NEngine^ NEngine::Instance()
	{
		return s_instance;
	}

	nengine* NEngine::NativePtr::get()
	{
		return dynamic_cast<nengine*>(RawPtr);		
	}

	void NEngine::InitCore()
	{
		BEGIN_NATIVE_GUARD		
			NativePtr->init_core();	
		END_NATIVE_GUARD

		m_levelDict = gcnew Dictionary<String^, NLevel^>();		
	}

	void NEngine::InitModules(NEngineConfig^ cfg)
	{
		BEGIN_NATIVE_GUARD

		engine_config native_cfg;
		cfg->ToNative( native_cfg );
		NativePtr->init_modules(native_cfg);
		m_config = cfg;

		//--
		NativeObjectBorrowed^ nativeFileSysWrapper 
			= gcnew NativeObjectBorrowed( NativePtr->get_file_sys() );
		m_fileSys = gcnew NFileSystem(nativeFileSysWrapper);

		
		//-- hold render resource manager
		NativeObjectBorrowed^ nativeRenderResManager = gcnew NativeObjectBorrowed(NativePtr->get_render_res_mgr());
		m_renderResourceManager = gcnew NRenderResourceManager( nativeRenderResManager );
		
		END_NATIVE_GUARD
	}

	void NEngine::Close()
	{
		NativePtr->close();
		delete s_instance;
		s_instance = nullptr;
	}

	void NEngine::BeginPlay()
	{
		NativePtr->begin_play();
	}

	NLevel^ NEngine::CreateLevel(System::String^ lvName, System::String^ nativeClassName)
	{
		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		pin_ptr<const wchar_t> szClassName = PtrToStringChars(nativeClassName);
		NLevel^ newLv = NLevel::FromNativePtr(
			NativePtr->create_level(szLvName, szClassName),
			nativeClassName );

		m_levelDict->Add(lvName, newLv);
		return newLv;
	}

	void NEngine::DestroyLevel(NLevel^ lv)
	{
		if(lv->NativePtr!=nullptr)
		{
			NativePtr->destroy_level( lv->GetNativeSmartPtr() );
			m_levelDict->Remove( lv->Name );
		}			
	}

	void NEngine::DestroyLevel(System::String^ lvName)
	{
		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->destroy_level( szLvName );
		m_levelDict->Remove( lvName );
	}

	NLevel^ NEngine::LoadLevel(NResourceLoc^ loc)
	{
		nlevel::ptr nlv;
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc->ToNative(nloc);

		nlv = NativePtr->load_level(nloc, NULL);
		END_NATIVE_GUARD

		System::String^ className = gcnew System::String(nlv->reflection_get_class()->get_name());
		NLevel^ mlv = NLevel::FromNativePtr(nlv, className);
		mlv->NativeLevelLoaded();
		m_levelDict->Add(mlv->Name, mlv);

		return mlv;
	}

	void NEngine::SaveLevel(System::String^ lvName, NResourceLoc^ loc)
	{
		nexus::resource_location nloc;
		loc->ToNative(nloc);

		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->save_level(szLvName,
			nloc, NULL);
	}

	void NEngine::ExportLevel( System::String^ lvName, NResourceLoc^ loc )
	{
		nexus::resource_location nloc;
		loc->ToNative(nloc);

		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->export_level(szLvName, nloc);
	}
}//namespace NexusEngine