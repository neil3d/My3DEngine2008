#include "stdafx.h"
//#include "../InternalHelpers.h"
#include "../Vector2.h"
#include "Vector2Converter.h"
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
		Vector2Converter::Vector2Converter()
		{
			Type^ type = Vector2::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("x")),
				gcnew FieldPropertyDescriptor(type->GetField("y")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool Vector2Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool Vector2Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ Vector2Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			Vector2^ vector = dynamic_cast<Vector2^>( value );

			if( destinationType == String::typeid && vector != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 2 );

				stringArray[0] = converter->ConvertToString( context, culture, vector->x );
				stringArray[1] = converter->ConvertToString( context, culture, vector->y );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && vector != nullptr )
			{
				ConstructorInfo^ info = (Vector2::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { vector->x, vector->y } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ Vector2Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );

			if( string != nullptr )
			{
				string = string->Trim();
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length != 2 )
					throw gcnew ArgumentException("Invalid vector format.");

				float X = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[0] ) );
				float Y = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[1] ) );

				return gcnew Vector2(X, Y);
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool Vector2Converter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		Object^ Vector2Converter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew Vector2( safe_cast<float>( propertyValues["x"] ), safe_cast<float>( propertyValues["y"] ) );
		}

		bool Vector2Converter::GetPropertiesSupported(ITypeDescriptorContext^)
		{
			return true;
		}

		PropertyDescriptorCollection^ Vector2Converter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
		{
			return m_Properties;
		}
	}
}