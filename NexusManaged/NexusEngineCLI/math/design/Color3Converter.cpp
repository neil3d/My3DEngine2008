#include "stdafx.h"
#include "../Color3.h"
#include "../Color4.h"
#include "Color3Converter.h"
#include "FieldPropertyDescriptor.h"

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
	Color3Converter::Color3Converter()
	{
		Type^ type = Color3::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("Red")),
			gcnew FieldPropertyDescriptor(type->GetField("Green")),
			gcnew FieldPropertyDescriptor(type->GetField("Blue")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool Color3Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool Color3Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ Color3Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		Color3^ color = dynamic_cast<Color3^>( value );

		if( destinationType == String::typeid && color != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 3 );

			stringArray[0] = converter->ConvertToString( context, culture, color->R );
			stringArray[1] = converter->ConvertToString( context, culture, color->G );
			stringArray[2] = converter->ConvertToString( context, culture, color->B );

			return String::Join( separator, stringArray );
		}
		else if( destinationType == InstanceDescriptor::typeid && color != nullptr )
		{
			ConstructorInfo^ info = (Color3::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid, float::typeid } );
			if( info != nullptr )
				return gcnew InstanceDescriptor( info, gcnew array<Object^> { color->R, color->G, color->B } );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ Color3Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
	{
		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		String^ string = dynamic_cast<String^>( value );

		if( string != nullptr )
		{
			string = string->Trim();
			TypeConverter^ floatConverter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

			if( stringArray->Length == 1 )
			{
				int number = 0;
				if( int::TryParse( string, number ) )
				{
					Color4f color( number );
					return gcnew Color3( color.R, color.G, color.B );
				}

				TypeConverter^ colorConverter = TypeDescriptor::GetConverter(Color::typeid);
				Color4f value( safe_cast<Color>( colorConverter->ConvertFromString( context, culture, string ) ) );
				return gcnew Color3( value.R, value.G, value.B );
			}
			else if( stringArray->Length == 3 )
			{
				int red;
				int green;
				int blue;
				if( int::TryParse( stringArray[0], red ) && int::TryParse( stringArray[1], green ) && int::TryParse( stringArray[2], blue ) )
				{
					Color4f color( Color::FromArgb( red, green, blue ) );
					return gcnew Color3( color.R, color.G, color.B );
				}

				float r = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[0] ) );
				float g = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );
				float b = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );

				return gcnew Color3( r, g, b );
			}
			else
				throw gcnew ArgumentException("Invalid color format.");
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool Color3Converter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);

		return true;
	}

	Object^ Color3Converter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		return gcnew Color3( safe_cast<float>( propertyValues["Red"] ),
			safe_cast<float>( propertyValues["Green"] ), safe_cast<float>( propertyValues["Blue"] ) );
	}

	bool Color3Converter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ Color3Converter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}