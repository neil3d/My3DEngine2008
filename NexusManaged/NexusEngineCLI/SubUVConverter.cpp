#include "StdAfx.h"
#include "SubUVConverter.h"
#include "subuv.h"
#include "math/design/FieldPropertyDescriptor.h"

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
		SubUVConverter::SubUVConverter()
		{
			Type^ type = NSubUV::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("HorizontalImageCount")),
				gcnew FieldPropertyDescriptor(type->GetField("VerticalImageCount")),
				gcnew FieldPropertyDescriptor(type->GetField("ImageIndex")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool SubUVConverter::CanConvertTo( System::ComponentModel::ITypeDescriptorContext^ context, System::Type^ destinationType )
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool SubUVConverter::CanConvertFrom( System::ComponentModel::ITypeDescriptorContext^ context, System::Type^ sourceType )
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		System::Object^ SubUVConverter::ConvertTo( System::ComponentModel::ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value, System::Type^ destinationType )
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			NSubUV^ vector = dynamic_cast<NSubUV^>( value );

			if( destinationType == String::typeid && vector != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(int::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 3 );

				stringArray[0] = converter->ConvertToString( context, culture, vector->HorizontalImageCount );
				stringArray[1] = converter->ConvertToString( context, culture, vector->VerticalImageCount );
				stringArray[2] = converter->ConvertToString( context, culture, vector->ImageIndex );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && vector != nullptr )
			{
				ConstructorInfo^ info = (NSubUV::typeid)->GetConstructor( gcnew array<Type^> { int::typeid, int::typeid, int::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { vector->HorizontalImageCount, vector->VerticalImageCount, vector->ImageIndex } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		System::Object^ SubUVConverter::ConvertFrom( System::ComponentModel::ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value )
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );

			if( string != nullptr )
			{
				string = string->Trim();
				TypeConverter^ converter = TypeDescriptor::GetConverter(int::typeid);
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length != 3 )
					throw gcnew ArgumentException("Invalid vector format.");

				int h = safe_cast<int>( converter->ConvertFromString( context, culture, stringArray[0] ) );
				int v = safe_cast<int>( converter->ConvertFromString( context, culture, stringArray[1] ) );
				int i = safe_cast<int>( converter->ConvertFromString( context, culture, stringArray[2] ) );

				return gcnew NSubUV(h,v,i);
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool SubUVConverter::GetCreateInstanceSupported( System::ComponentModel::ITypeDescriptorContext^ context )
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		System::Object^ SubUVConverter::CreateInstance( System::ComponentModel::ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues )
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew NSubUV( safe_cast<int>( propertyValues["HorizontalImageCount"] ), safe_cast<int>( propertyValues["VerticalImageCount"] ),
				safe_cast<int>( propertyValues["ImageIndex"] ) );
		}

		bool SubUVConverter::GetPropertiesSupported( System::ComponentModel::ITypeDescriptorContext^ context )
		{
			return true;
		}

		System::ComponentModel::PropertyDescriptorCollection^ SubUVConverter::GetProperties( System::ComponentModel::ITypeDescriptorContext^ context, System::Object^ value, array<System::Attribute^>^ attributes )
		{
			return m_Properties;
		}
	}
}