/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	//!	文件对象的类型
	public enum class EFileEntityType : System::Int32
	{
		Invalid		= EFile_Invalid,	// 无效值
		Regular		= EFile_Regular,	// 普通文件
		Folder		= EFile_Folder		// 文件夹
	};

	//!	文件对象信息
	public value struct NFileEntity
	{
		EFileEntityType type;		// 类型
		System::String^	pkg;		// 所在的package
		System::String^	path;		// 路径
		System::UInt32	fileSize;	// 文件大小 - 仅对普通文件可用

		property String^ PackageName
		{
			String^ get()	{	return pkg; }
		}

		property String^ FilePath
		{
			String^ get()	{	return path; }
		}

		property String^ FileName
		{
			String^ get();
		}

		property String^ FileNameNoExtension
		{
			String^ get();
		}

		property String^ FileExtension
		{
			String^ get();
		}

		static NFileEntity^ FromNative(const nfile_entity& file);
	};

	/**
	 *	文件查询接口
	*/
	public ref class NFileQuery abstract
	{
	public:
		NFileQuery(void);

		//! 文件系统找到某个文件对象时的回调函数
		virtual void OnFileEntity(NFileEntity% file) abstract;
	};

	//-- Native包装,使得Native代码可以调用Managed代码
	class NativeFileQueryWrapper : public nexus::nfile_query
	{
		gcroot<NFileQuery^>		m_objCLR;
	public:
		NativeFileQueryWrapper(NFileQuery^ clrObj);
		virtual void on_file_entity(const nfile_entity& file);
	};
}//namespace NexusEngine