#include "stdafx.h"
#include "FontDescription.h"
#include "FontDescriptionConverter.h"
#include "math/design/FieldPropertyDescriptor.h"

using namespace System;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::ComponentModel::Design::Serialization;
using namespace System::Globalization;
using namespace System::Reflection;

namespace NexusEngine
{
	namespace Design
	{
		FontDescriptionConverter::FontDescriptionConverter()
		{
			Type^ type = FontDescription::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("Height")),
				gcnew FieldPropertyDescriptor(type->GetField("Width")),
				gcnew FieldPropertyDescriptor(type->GetField("Weight")),
				gcnew FieldPropertyDescriptor(type->GetField("MipLevels")),
				gcnew FieldPropertyDescriptor(type->GetField("Italic")),
				gcnew FieldPropertyDescriptor(type->GetField("CharSet")),
				gcnew FieldPropertyDescriptor(type->GetField("OutputPrecision")),
				gcnew FieldPropertyDescriptor(type->GetField("Quality")),
				gcnew FieldPropertyDescriptor(type->GetField("PitchAndFamily")),
				gcnew FieldPropertyDescriptor(type->GetField("FaceName"))
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		Object^ FontDescriptionConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			FontDescription^ desc = dynamic_cast<FontDescription^>( value );
			if( destinationType == String::typeid && desc != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				array<String^>^ stringArray = gcnew array<String^>( 10 );

				TypeConverter^ intConverter = TypeDescriptor::GetConverter(int::typeid);
				stringArray[0] = intConverter->ConvertToString( context, culture, desc->Height );
				stringArray[1] = intConverter->ConvertToString( context, culture, desc->Width );
				stringArray[2] = TypeDescriptor::GetConverter(FontWeight::typeid)->ConvertToString( context, culture, desc->Weight );
				stringArray[3] = intConverter->ConvertToString( context, culture, desc->MipLevels );
				stringArray[4] = TypeDescriptor::GetConverter(bool::typeid)->ConvertToString( context, culture, desc->Italic );
				stringArray[5] = TypeDescriptor::GetConverter(CharacterSet::typeid)->ConvertToString( context, culture, desc->CharSet );
				stringArray[6] = TypeDescriptor::GetConverter(Precision::typeid)->ConvertToString( context, culture, desc->OutputPrecision );
				stringArray[7] = TypeDescriptor::GetConverter(FontQuality::typeid)->ConvertToString( context, culture, desc->Quality );
				stringArray[8] = TypeDescriptor::GetConverter(PitchAndFamily::typeid)->ConvertToString( context, culture, desc->PitchAndFamily );
				stringArray[9] = desc->FaceName;

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && desc != nullptr )
			{
				ConstructorInfo^ info = (FontDescription::typeid)->GetConstructor( gcnew array<Type^> { } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> {} );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		bool FontDescriptionConverter::CanConvertFrom( System::ComponentModel::ITypeDescriptorContext^ context, System::Type^ sourceType )
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		System::Object^ FontDescriptionConverter::ConvertFrom( System::ComponentModel::ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, System::Object^ value )
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );
			if( string != nullptr )
			{
				string = string->Trim();
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length == 10 )
				{
					TypeConverter^ intConverter = TypeDescriptor::GetConverter(int::typeid);
					FontDescription^ desc = gcnew FontDescription();
						
					desc->Height = safe_cast<int>( intConverter->ConvertFromString( context, culture, stringArray[0] ) );
					desc->Width = safe_cast<int>( intConverter->ConvertFromString( context, culture, stringArray[1] ) );
					desc->Weight = safe_cast<FontWeight>(TypeDescriptor::GetConverter(FontWeight::typeid)->ConvertFromString( context, culture, stringArray[2] ) );
					desc->MipLevels = safe_cast<int>( intConverter->ConvertFromString( context, culture, stringArray[3] ) );
					desc->Italic = safe_cast<bool>(TypeDescriptor::GetConverter(bool::typeid)->ConvertFromString( context, culture, stringArray[4] ) );
					desc->CharSet = safe_cast<CharacterSet>(TypeDescriptor::GetConverter(CharacterSet::typeid)->ConvertFromString( context, culture, stringArray[5] ) );
					desc->OutputPrecision = safe_cast<Precision>(TypeDescriptor::GetConverter(Precision::typeid)->ConvertFromString( context, culture, stringArray[6] ) );
					desc->Quality = safe_cast<FontQuality>(TypeDescriptor::GetConverter(FontQuality::typeid)->ConvertFromString( context, culture, stringArray[7] ) );
					desc->PitchAndFamily = safe_cast<PitchAndFamily>(TypeDescriptor::GetConverter(PitchAndFamily::typeid)->ConvertFromString( context, culture, stringArray[8] ) );
					desc->FaceName = stringArray[9];

					return desc;
				}
				else
					throw gcnew ArgumentException("Invalid color format.");
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool FontDescriptionConverter::GetPropertiesSupported( System::ComponentModel::ITypeDescriptorContext^ context )
		{
			UNREFERENCED_PARAMETER(context);
			return true;
		}

		bool FontDescriptionConverter::GetCreateInstanceSupported( System::ComponentModel::ITypeDescriptorContext^ context )
		{
			UNREFERENCED_PARAMETER(context);
			return true;
		}

		System::ComponentModel::PropertyDescriptorCollection^ FontDescriptionConverter::GetProperties( System::ComponentModel::ITypeDescriptorContext^ context, System::Object^ value, array<System::Attribute^>^ attributes )
		{
			FontDescription^ desc = dynamic_cast<FontDescription^>( value );
			if(desc != nullptr) 
				return   m_Properties;

			return   ExpandableObjectConverter::GetProperties(context,   value,   attributes); 
		}

		bool FontDescriptionConverter::CanConvertTo( System::ComponentModel::ITypeDescriptorContext^ context, System::Type^ destinationType )
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		System::Object^ FontDescriptionConverter::CreateInstance( System::ComponentModel::ITypeDescriptorContext^ context, System::Collections::IDictionary^ propertyValues )
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			FontDescription^ desc = gcnew FontDescription();

			desc->Height = safe_cast<int>( propertyValues["Height"] );
			desc->Width = safe_cast<int>( propertyValues["Width"] );
			desc->Weight = safe_cast<FontWeight>( propertyValues["Weight"] );
			desc->MipLevels = safe_cast<int>( propertyValues["MipLevels"] );
			desc->Italic = safe_cast<bool>( propertyValues["Italic"] );
			desc->CharSet = safe_cast<CharacterSet>( propertyValues["CharSet"] );
			desc->OutputPrecision = safe_cast<Precision>( propertyValues["OutputPrecision"] );
			desc->Quality = safe_cast<FontQuality>( propertyValues["Quality"] );
			desc->PitchAndFamily = safe_cast<PitchAndFamily>( propertyValues["PitchAndFamily"] );
			desc->FaceName = safe_cast<System::String^>( propertyValues["FaceName"] );
			return desc;
		}
	}
}