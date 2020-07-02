/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NFileQuery.h"

namespace NexusEngine
{
	/*
	 *	文件系统所打开的一个文件实例, 包装nexus::nfile
	*/
	public ref class NFile : public NativeObjectOwnedBySharedPtr
	{
		//-- Native wrapper
	public:
		explicit NFile(nexus::nfile::ptr nativeObj);
		property nexus::nfile* NativePtr
		{	nexus::nfile* get(); }
	};

	/**
	 *	包装Native class ‘nexus::nfile_system’
	*/
	public ref class NFileSystem
	{
	public:
		//!	文件系统中有多少个文件包
		int GetNumPackage();

		//!	查询某个文件包的名称
		System::String^ GetPackageName(int i);

		//!	查询某个文件包中的某个路径下的所有文件和文件夹
		void QueryPackage(System::String^ pkgName, System::String^ path, NFileQuery^ fq);

		System::UInt32 GetFileSize(System::String^ pkgName, System::String^ path);
		System::DateTime GetFileTime(System::String^ pkgName, System::String^ path);

		//-- Native wrapper
	public:
		/** 内部构造一个Native nfile_system对象，其独立管理生存期*/
		explicit NFileSystem(System::String^ nativeClassName);
		/** 有外部传入Native nfile_system对象，外部决定生存期管理方式*/
		explicit NFileSystem(INativeObj^ nativeObj);

		property nfile_system* NativePtr
		{
			nfile_system* get();
		}
	private:
		INativeObj^	m_nativeObj;
	};
}//namespace NexusCore