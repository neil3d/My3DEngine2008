#include "stdafx.h"
#include "ColorRect.h"
#include "ColorRectConverter.h"
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
		ColorRectConverter::ColorRectConverter()
		{
			Type^ type = ColorRect::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("left_top")),
				gcnew FieldPropertyDescriptor(type->GetField("right_top")),
				gcnew FieldPropertyDescriptor(type->GetField("left_bottom")),
				gcnew FieldPropertyDescriptor(type->GetField("right_bottom")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool ColorRectConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool ColorRectConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ ColorRectConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			ColorRect^ colorRC = dynamic_cast<ColorRect^>( value );

			if( destinationType == String::typeid && colorRC != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(Color4f::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 4 );

				stringArray[0] = converter->ConvertToString( context, culture, colorRC->left_top );
				stringArray[1] = converter->ConvertToString( context, culture, colorRC->right_top );
				stringArray[2] = converter->ConvertToString( context, culture, colorRC->left_bottom );
				stringArray[3] = converter->ConvertToString( context, culture, colorRC->right_bottom );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && colorRC != nullptr )
			{
				ConstructorInfo^ info = (ColorRect::typeid)->GetConstructor( gcnew array<Type^> { Color4f::typeid, Color4f::typeid, Color4f::typeid, Color4f::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { colorRC->left_top, colorRC->right_top, colorRC->left_bottom,colorRC->right_bottom } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ ColorRectConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );

			if( string != nullptr )
			{
				string = string->Trim();
				TypeConverter^ floatConverter = TypeDescriptor::GetConverter(Color4f::typeid);
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length == 4 )
				{
					Color4f left_top = safe_cast<Color4f>( floatConverter->ConvertFromString( context, culture, stringArray[0] ) );
					Color4f right_top = safe_cast<Color4f>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );
					Color4f left_bottom = safe_cast<Color4f>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );
					Color4f right_bottom = safe_cast<Color4f>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );

					return gcnew ColorRect( left_top, right_top, left_bottom,right_bottom );
				}
				else
					throw gcnew ArgumentException("Invalid color format.");
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool ColorRectConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		Object^ ColorRectConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew ColorRect( safe_cast<Color4f>( propertyValues["left_top"] ),
				safe_cast<Color4f>( propertyValues["right_top"] ), 
				safe_cast<Color4f>( propertyValues["left_bottom"] ), 
				safe_cast<Color4f>( propertyValues["right_bottom"] ) );
		}

		bool ColorRectConverter::GetPropertiesSupported(ITypeDescriptorContext^)
		{
			return true;
		}

		PropertyDescriptorCollection^ ColorRectConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
		{
			return m_Properties;
		}
	}
}