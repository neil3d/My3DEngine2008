#include "stdafx.h"
#include "Color.h"
#include "ColorConverter.h"
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
		Color4ubConverter::Color4ubConverter()
		{
			Type^ type = Color4ub::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("R")),
				gcnew FieldPropertyDescriptor(type->GetField("G")),
				gcnew FieldPropertyDescriptor(type->GetField("B")),
				gcnew FieldPropertyDescriptor(type->GetField("A")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool Color4ubConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool Color4ubConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ Color4ubConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			Color4ub^ vector = dynamic_cast<Color4ub^>( value );

			if( destinationType == String::typeid && vector != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 4 );

				stringArray[0] = converter->ConvertToString( context, culture, vector->R );
				stringArray[1] = converter->ConvertToString( context, culture, vector->G );
				stringArray[2] = converter->ConvertToString( context, culture, vector->B );
				stringArray[3] = converter->ConvertToString( context, culture, vector->A );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && vector != nullptr )
			{
				ConstructorInfo^ info = (Color4ub::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid, float::typeid, float::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { vector->R, vector->G, vector->B, vector->A } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ Color4ubConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );

			if( string != nullptr )
			{
				string = string->Trim();
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length != 4 )
					throw gcnew ArgumentException("Invalid vector format.");

				System::Byte R = safe_cast<System::Byte>( converter->ConvertFromString( context, culture, stringArray[0] ) );
				System::Byte G = safe_cast<System::Byte>( converter->ConvertFromString( context, culture, stringArray[1] ) );
				System::Byte B = safe_cast<System::Byte>( converter->ConvertFromString( context, culture, stringArray[2] ) );
				System::Byte A = safe_cast<System::Byte>( converter->ConvertFromString( context, culture, stringArray[3] ) );

				return gcnew Color4ub(R, G, B, A);
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool Color4ubConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		Object^ Color4ubConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew Color4ub( safe_cast<System::Byte>( propertyValues["R"] ), safe_cast<System::Byte>( propertyValues["G"] ),
				safe_cast<System::Byte>( propertyValues["B"] ), safe_cast<System::Byte>( propertyValues["A"] ) );
		}

		bool Color4ubConverter::GetPropertiesSupported(ITypeDescriptorContext^)
		{
			return true;
		}

		PropertyDescriptorCollection^ Color4ubConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
		{
			return m_Properties;
		}
	}
}