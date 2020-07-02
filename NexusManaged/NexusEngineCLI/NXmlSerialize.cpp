#include "StdAfx.h"
#include "NXmlSerialize.h"
#include "NEngine.h"
#include "NFileSystem.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	

	NXmlSerializeBase::NXmlSerializeBase( narchive::ptr& nativeArchive )
	{
		m_smart_ptr = new nexus::narchive::ptr( nativeArchive );
	}

	NXmlSerializeBase::~NXmlSerializeBase()
	{
		if(m_smart_ptr)
		{
			delete m_smart_ptr;
			m_smart_ptr = NULL;
		}
	}

	nexus::narchive* NXmlSerializeBase::NativePtr::get()
	{
		if( m_smart_ptr )				
			return m_smart_ptr->get();
		else
			return NULL;
	}

	nexus::narchive::ptr* NXmlSerializeBase::SharedPtr::get()
	{
		return m_smart_ptr;
	}

	void NXmlSerializeBase::ObjectBegin( String^ objectName )
	{
		pin_ptr<const wchar_t> nativeName = PtrToStringChars(objectName);
		nstring nativeClass(_T("NonNativeClass"));
		NativePtr->object_begin(nativeName,nativeClass);
	}

	void NXmlSerializeBase::ArrayBegin( String^ arrayName,[Out] int% arraySize )
	{
		pin_ptr<const wchar_t> nativeName = PtrToStringChars(arrayName);
		size_t nativeSize = arraySize;
		NativePtr->array_begin(nativeName, nativeSize);
		arraySize = nativeSize;
	}

	void NXmlSerializeBase::ArrayNext()
	{
		NativePtr->array_next();
	}

	void NXmlSerializeBase::ArrayEnd()
	{
		NativePtr->array_end();
	}

	void NXmlSerializeBase::Serial( String^ key, String^% value )
	{
		pin_ptr<const wchar_t> nativeKey = PtrToStringChars(key);
		pin_ptr<const wchar_t> nativeValue = PtrToStringChars(value);

		nstring nativeObject(nativeValue);
		NativePtr->serial(nativeKey, nativeObject);

		value = gcnew String(nativeObject.c_str()); 
	}

	void NXmlSerializeBase::ObjectEnd()
	{
		NativePtr->object_end();
	}

	void NXmlSerializeBase::Close()
	{
		NativePtr->close();
	}

	NXmlSerializeReader::NXmlSerializeReader( System::String^ pkgName, System::String^ fileName )
		: NXmlSerializeBase()
	{
		pin_ptr<const wchar_t> native_pkg_name = PtrToStringChars(pkgName);
		pin_ptr<const wchar_t> native_file_name = PtrToStringChars(fileName);

		BEGIN_NATIVE_GUARD

		narchive::ptr nativeArchive = narchive::open_xml_archive(
			NEngine::Instance()->FileSystem->NativePtr, EFileRead, native_pkg_name, native_file_name);
		
		m_smart_ptr = new nexus::narchive::ptr( nativeArchive );
		END_NATIVE_GUARD
	}

	String^ NXmlSerializeReader::ObjectDescription( String^ key )
	{
		pin_ptr<const wchar_t> nativeKey = PtrToStringChars(key);

		nstring nativeObject;
		NativePtr->object_description(nativeKey,nativeObject);

		return gcnew String(nativeObject.c_str());
	}

	NXmlSerializeWriter::NXmlSerializeWriter( System::String^ pkgName, System::String^ fileName )
		: NXmlSerializeBase()
	{
		pin_ptr<const wchar_t> native_pkg_name = PtrToStringChars(pkgName);
		pin_ptr<const wchar_t> native_file_name = PtrToStringChars(fileName);

		BEGIN_NATIVE_GUARD

		narchive::ptr nativeArchive = narchive::open_xml_archive(
			NEngine::Instance()->FileSystem->NativePtr, EFileWrite, native_pkg_name, native_file_name);

		m_smart_ptr = new nexus::narchive::ptr( nativeArchive );
		END_NATIVE_GUARD
	}

	void NXmlSerializeWriter::ObjectDescription( String^ key, String^ value )
	{
		pin_ptr<const wchar_t> nativeKey = PtrToStringChars(key);
		pin_ptr<const wchar_t> nativeValue = PtrToStringChars(value);

		nstring nativeObject(nativeValue);
		NativePtr->object_description(nativeKey, nativeObject);
	}
}