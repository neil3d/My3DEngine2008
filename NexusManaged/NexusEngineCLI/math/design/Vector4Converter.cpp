#include "stdafx.h"
//#include "../InternalHelpers.h"
#include "../Vector4.h"
#include "Vector4Converter.h"
#include "FieldPropertyDescriptor.h"

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
		Vector4Converter::Vector4Converter()
		{
			Type^ type = Vector4::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("x")),
				gcnew FieldPropertyDescriptor(type->GetField("y")),
				gcnew FieldPropertyDescriptor(type->GetField("z")),
				gcnew FieldPropertyDescriptor(type->GetField("w")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool Vector4Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool Vector4Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ Vector4Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			Vector4^ vector = dynamic_cast<Vector4^>( value );

			if( destinationType == String::typeid && vector != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 4 );

				stringArray[0] = converter->ConvertToString( context, culture, vector->x );
				stringArray[1] = converter->ConvertToString( context, culture, vector->y );
				stringArray[2] = converter->ConvertToString( context, culture, vector->z );
				stringArray[3] = converter->ConvertToString( context, culture, vector->w );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && vector != nullptr )
			{
				ConstructorInfo^ info = (Vector4::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid, float::typeid, float::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { vector->x, vector->y, vector->z, vector->w } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ Vector4Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
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

				float X = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[0] ) );
				float Y = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[1] ) );
				float Z = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[2] ) );
				float W = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[3] ) );

				return gcnew Vector4(X, Y, Z, W);
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool Vector4Converter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		Object^ Vector4Converter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew Vector4( safe_cast<float>( propertyValues["x"] ), safe_cast<float>( propertyValues["y"] ),
				safe_cast<float>( propertyValues["z"] ), safe_cast<float>( propertyValues["w"] ) );
		}

		bool Vector4Converter::GetPropertiesSupported(ITypeDescriptorContext^)
		{
			return true;
		}

		PropertyDescriptorCollection^ Vector4Converter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
		{
			return m_Properties;
		}
	}
}