/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NLevel.h"
#include "RenderDefine.h"
#include "NResource.h"

namespace NexusEngine
{
	ref class NFileSystem;
	ref class NRenderElement;	

	//!	引擎配置
	public ref struct NEngineConfig
	{
		//-- 渲染设置
		System::IntPtr	RenderWndHandle;
		System::Int32	ClientWidth;
		System::Int32	ClientHeight;
		System::Byte	ColorBits;
		bool			FullScreen;
		bool			EnableHDR;

		//-- Native Engine设置
		System::String^	RenderClass;
		System::String^ FileSystemClass;
		System::String^ FileSystemRoot;
		System::String^ EngineDataPkg;

		void ToNative(engine_config& cfg);
	};
	
	/**
	 *	提供对Native Class ‘nengine’的包装
	*/
	public ref class NEngine : public NativeObjectOwnedRaw
	{
	public:
		NEngine(void);		

		static NEngine^ Instance();

		//-- 引擎启动--关闭接口 -------------------------------------------------------
		//! 首先要初始化核心模块
		virtual void InitCore();

		//!	然后创建其他模块
		virtual void InitModules(NEngineConfig^ cfg);

		//! 关闭引擎,执行最后的清理
		virtual void Close();

		//-- 关卡管理接口 -------------------------------------------------------------
		//!	创建一个新的空白关卡,关卡的名称要求是唯一的
		NLevel^ CreateLevel(System::String^ lvName, System::String^ nativeClassName);

		//! 销毁某个Level, 相当于Dispose, 即使外部仍持有NLevel对象引用, 其内容也已经不可用;
		void DestroyLevel(NLevel^ lv);

		//!	销毁指定名称的Level
		void DestroyLevel(System::String^ lvName);

		//!	从指定的文件系统目录读入关卡
		NLevel^ LoadLevel(NResourceLoc^ loc);

		//!	将指定名称的Level存入文件系统的某个目录
		void SaveLevel(System::String^ lvName, NResourceLoc^ loc);

		property NFileSystem^ FileSystem
		{
			NFileSystem^ get()	{	return m_fileSys;}
		}
		
		property NEngineConfig^ Config
		{
			NEngineConfig^ get()	{	return m_config;}			
		}
	private:
		static NEngine^ s_instance;
		NFileSystem^	m_fileSys;
		NEngineConfig^	m_config;

	protected:
		System::Collections::Generic::Dictionary<System::String^, NLevel^>^	m_levelDict;

		//-- Native Wrapper
	protected:
		NEngine(System::String^ nativeClassName);
		property nengine* NativePtr
		{
			nengine* get();
		}
	};
}//namespace NexusEngine