#include "stdafx.h"
#include "../Color4.h"
#include "Color4Converter.h"
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
	Color4Converter::Color4Converter()
	{
		Type^ type = Color4f::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("R")),
			gcnew FieldPropertyDescriptor(type->GetField("G")),
			gcnew FieldPropertyDescriptor(type->GetField("B")),
			gcnew FieldPropertyDescriptor(type->GetField("A")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool Color4Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool Color4Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ Color4Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		Color4f^ color = dynamic_cast<Color4f^>( value );

		if( destinationType == String::typeid && color != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 4 );

			stringArray[0] = converter->ConvertToString( context, culture, color->A );
			stringArray[1] = converter->ConvertToString( context, culture, color->R );
			stringArray[2] = converter->ConvertToString( context, culture, color->G );
			stringArray[3] = converter->ConvertToString( context, culture, color->B );

			return String::Join( separator, stringArray );
		}
		else if( destinationType == InstanceDescriptor::typeid && color != nullptr )
		{
			ConstructorInfo^ info = (Color4f::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid, float::typeid, float::typeid } );
			if( info != nullptr )
				return gcnew InstanceDescriptor( info, gcnew array<Object^> { color->A, color->R, color->G, color->B } );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ Color4Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
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
					return gcnew Color4f( number );

				TypeConverter^ colorConverter = TypeDescriptor::GetConverter(Color::typeid);
				return gcnew Color4f( safe_cast<Color>( colorConverter->ConvertFromString( context, culture, string ) ) );
			}
			else if( stringArray->Length == 3 )
			{
				int red;
				int green;
				int blue;
				if( int::TryParse( stringArray[0], red ) && int::TryParse( stringArray[1], green ) && int::TryParse( stringArray[2], blue ) )
					return gcnew Color4f( Color::FromArgb( red, green, blue ) );

				float r = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[0] ) );
				float g = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );
				float b = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );

				return gcnew Color4f( r, g, b );
			}
			else if( stringArray->Length == 4 )
			{
				int red;
				int green;
				int blue;
				int alpha;
				if( int::TryParse( stringArray[0], alpha ) && int::TryParse( stringArray[1], red ) && int::TryParse( stringArray[2], green ) && int::TryParse( stringArray[3], blue ) )
					return gcnew Color4f( Color::FromArgb( alpha, red, green, blue ) );

				float a = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[0] ) );
				float r = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );
				float g = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );
				float b = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[3] ) );

				return gcnew Color4f( a, r, g, b );
			}
			else
				throw gcnew ArgumentException("Invalid color format.");
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool Color4Converter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);

		return true;
	}

	Object^ Color4Converter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		return gcnew Color4f( safe_cast<float>( propertyValues["A"] ), safe_cast<float>( propertyValues["R"] ),
			safe_cast<float>( propertyValues["G"] ), safe_cast<float>( propertyValues["B"] ) );
	}

	bool Color4Converter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ Color4Converter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}