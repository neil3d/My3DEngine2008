#include "StdAfx.h"
#include "NResource.h"
#include "NWrapperException.h"

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::ComponentModel::Design::Serialization;
using namespace System::Globalization;
using namespace System::Reflection;


namespace NexusEngine
{
	namespace Design
	{
		//-- class NResourceLocConverter ------------------------------------------------------
		bool NResourceLocConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool NResourceLocConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ NResourceLocConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			NResourceLoc^ loc = dynamic_cast<NResourceLoc^>( value );

			if( destinationType == String::typeid && loc != nullptr )
			{
				return loc->ToString();
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ NResourceLocConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
		{
			String^ string = dynamic_cast<String^>( value );
			if( string != nullptr )
			{
				string = string->Trim();
				return gcnew NResourceLoc(string);
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}
	}//namespace Design

	//-- class NResrouceLoc ------------------------------------------------------
	NResourceLoc::NResourceLoc(System::String^ loc)
	{
		if(String::IsNullOrEmpty(loc))
		{
			pkgName = String::Empty;
			fileName = String::Empty;
			return;
		}

		int pos = loc->IndexOf(':');
		if (pos != -1)
		{
			pkgName = loc->Substring(0, pos);
			fileName = loc->Substring(pos+1);
		}
		else
		{
			//throw gcnew System::ArgumentException("Bad Resource Location");
		}
	}

	NResourceLoc::NResourceLoc( const nexus::resource_location& nativeLoc )
	{
		pkgName = gcnew System::String( nativeLoc.pkg_name.c_str() );
		fileName = gcnew System::String( nativeLoc.file_name.c_str() );
	}

	void NResourceLoc::FromNative( const nexus::resource_location& nativeLoc )
	{
		pkgName = gcnew System::String( nativeLoc.pkg_name.c_str() );
		fileName = gcnew System::String( nativeLoc.file_name.c_str() );
	}

	void NResourceLoc::ToNative(nexus::resource_location& nativeLoc)
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(fileName);

		nativeLoc.pkg_name = String::IsNullOrEmpty(pkgName)?L"":szPkgName;
		nativeLoc.file_name = String::IsNullOrEmpty(fileName)?L"":szFileName;;
	}

	System::String^ NResourceLoc::ToString()
	{
		if (pkgName == nullptr
			|| fileName == nullptr)
		{
			return gcnew System::String("None");
		}

		if( pkgName->Length <= 0
			|| fileName->Length <= 0)
		{
			return gcnew System::String("None");
		}
		else
		{
			System::String^ ret = pkgName;
			ret += ":";
			ret += fileName;
			return ret;
		}
	}

	String^ NResourceLoc::FileExtension::get()
	{
		String^ ret = gcnew String(fileName);

		int p = ret->LastIndexOf(_T('.'));
		if( p != -1)
			ret = ret->Substring(p+1);

		ret->ToLower();
		return ret;
	}

	bool NResourceLoc::operator == ( NResourceLoc left, NResourceLoc right )
	{
		return NResourceLoc::Equals( left, right );
	}

	bool NResourceLoc::operator != ( NResourceLoc left, NResourceLoc right )
	{
		return !NResourceLoc::Equals( left, right );
	}

	int NResourceLoc::GetHashCode()
	{
		return (pkgName==nullptr ? 0 : pkgName->GetHashCode())
			+ (fileName==nullptr ? 0 : fileName->GetHashCode());
	}

	bool NResourceLoc::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<NResourceLoc>( value ) );
	}

	bool NResourceLoc::Equals( NResourceLoc value )
	{
		return ( pkgName == value.pkgName && fileName == value.fileName );
	}

	bool NResourceLoc::Equals( NResourceLoc% value1, NResourceLoc% value2 )
	{
		return ( value1.pkgName == value2.pkgName && value1.fileName == value2.fileName );
	}


	//-- class NResource ------------------------------------------------------
	NResource::NResource(nresource::ptr nativeResPtr)
	{
		m_nativeResPtr = new nresource::ptr(nativeResPtr);
	}

	NResource::~NResource(void)
	{
		delete m_nativeResPtr;
		m_nativeResPtr = NULL;
	}

	nresource* NResource::NativePtr::get()
	{
		return m_nativeResPtr->get();
	}

	NResourceLoc NResource::Location::get()
	{
		return NResourceLoc( NativePtr->get_location() );
	}

	System::String^ NResource::Name::get()
	{
		return gcnew System::String( NativePtr->get_name_str().c_str() );
	}

	void NResource::LoadFromFile(NResourceLoc loc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nativeLoc;
		loc.ToNative(nativeLoc);
		NativePtr->load_from_file( nativeLoc );

		END_NATIVE_GUARD
	}

	void NResource::SaveToFile(NResourceLoc loc, bool xml)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nativeLoc;
		loc.ToNative(nativeLoc);
		NativePtr->save_to_file( nativeLoc, xml );

		END_NATIVE_GUARD
	}

	void NResource::PostEditChange(bool ready)
	{
		NativePtr->post_edit_change(true);
	}
}//namespace NexusEngine