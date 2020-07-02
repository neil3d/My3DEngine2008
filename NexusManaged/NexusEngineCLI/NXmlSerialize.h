#pragma once
#include "NFileSystem.h"

namespace NexusEngine
{
	using namespace NexusEngineExtension;

	public ref class NXmlSerializeBase
	{
	public:
		explicit NXmlSerializeBase(narchive::ptr& nativeArchive);
		NXmlSerializeBase(void):m_smart_ptr(NULL)
		{}
		~NXmlSerializeBase();		

		void Close();

		virtual void ObjectBegin(String^ objectName);

		// 提供容器的便利接口
		virtual void ArrayBegin(String^ arrayName, [Out] int% arraySize);
		virtual void ArrayNext(); 
		virtual void ArrayEnd();

		// 只提供给managed code string的序列化足以
		virtual void Serial(String^ key,[Out] String^% value);

		virtual void ObjectEnd();

	protected:
		property narchive* NativePtr
		{
			narchive* get();
		}

		property narchive::ptr* SharedPtr
		{
			narchive::ptr* get();
		}

	protected:
		narchive::ptr*	m_smart_ptr;
	};

	/**
	* 对Native narchive_rapidxml_reader封装
	* 提供给Managed code序列化接口
	*/
	public ref class NXmlSerializeReader 
		: public NXmlSerializeBase
	{
	public:
		NXmlSerializeReader(System::String^ pkgName, System::String^ fileName);

		virtual String^ ObjectDescription(String^ key);
	};

	/**
	* 对Native narchive_rapidxml_writer封装
	* 提供给Managed code序列化接口
	*/
	public ref class NXmlSerializeWriter 
		: public NXmlSerializeBase
	{
	public:
		NXmlSerializeWriter(System::String^ pkgName, System::String^ fileName);

		virtual void ObjectDescription(String^ key, String^ value);
	};
}
