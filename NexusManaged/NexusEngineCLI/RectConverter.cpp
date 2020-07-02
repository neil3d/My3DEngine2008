#include "stdafx.h"
#include "Rect.h"
#include "RectConverter.h"
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
		RectConverter::RectConverter()
		{
			Type^ type = Rect::typeid;
			array<PropertyDescriptor^>^ propArray =
			{
				gcnew FieldPropertyDescriptor(type->GetField("left")),
				gcnew FieldPropertyDescriptor(type->GetField("top")),
				gcnew FieldPropertyDescriptor(type->GetField("right")),
				gcnew FieldPropertyDescriptor(type->GetField("bottom")),
			};

			m_Properties = gcnew PropertyDescriptorCollection(propArray);
		}

		bool RectConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
		{
			if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertTo(context, destinationType);
		}

		bool RectConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
		{
			if( sourceType == String::typeid )
				return true;
			else
				return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
		}

		Object^ RectConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
		{
			if( destinationType == nullptr )
				throw gcnew ArgumentNullException( "destinationType" );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			Rect^ rc = dynamic_cast<Rect^>( value );

			if( destinationType == String::typeid && rc != nullptr )
			{
				String^ separator = culture->TextInfo->ListSeparator + " ";
				TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = gcnew array<String^>( 4 );

				stringArray[0] = converter->ConvertToString( context, culture, rc->left );
				stringArray[1] = converter->ConvertToString( context, culture, rc->top );
				stringArray[2] = converter->ConvertToString( context, culture, rc->right );
				stringArray[3] = converter->ConvertToString( context, culture, rc->bottom );

				return String::Join( separator, stringArray );
			}
			else if( destinationType == InstanceDescriptor::typeid && rc != nullptr )
			{
				ConstructorInfo^ info = (Rect::typeid)->GetConstructor( gcnew array<Type^> { float::typeid, float::typeid, float::typeid } );
				if( info != nullptr )
					return gcnew InstanceDescriptor( info, gcnew array<Object^> { rc->left, rc->top, rc->right,rc->bottom } );
			}

			return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
		}

		Object^ RectConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
		{
			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String^ string = dynamic_cast<String^>( value );

			if( string != nullptr )
			{
				string = string->Trim();
				TypeConverter^ floatConverter = TypeDescriptor::GetConverter(float::typeid);
				array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

				if( stringArray->Length == 4 )
				{
					float left_top = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[0] ) );
					float right_top = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );
					float left_bottom = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );
					float right_bottom = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[2] ) );

					return gcnew Rect( left_top, right_top, left_bottom,right_bottom );
				}
				else
					throw gcnew ArgumentException("Invalid rect format.");
			}

			return ExpandableObjectConverter::ConvertFrom(context, culture, value);
		}

		bool RectConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
		{
			UNREFERENCED_PARAMETER(context);

			return true;
		}

		Object^ RectConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
		{
			UNREFERENCED_PARAMETER(context);

			if( propertyValues == nullptr )
				throw gcnew ArgumentNullException( "propertyValues" );

			return gcnew Rect( safe_cast<float>( propertyValues["left"] ),
				safe_cast<float>( propertyValues["top"] ), 
				safe_cast<float>( propertyValues["right"] ), 
				safe_cast<float>( propertyValues["bottom"] ) );
		}

		bool RectConverter::GetPropertiesSupported(ITypeDescriptorContext^)
		{
			return true;
		}

		PropertyDescriptorCollection^ RectConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
		{
			return m_Properties;
		}
	}
}