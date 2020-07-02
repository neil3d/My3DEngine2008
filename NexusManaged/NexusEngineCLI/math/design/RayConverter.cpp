#include "stdafx.h"
#include "../Ray.h"
#include "RayConverter.h"
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
	RayConverter::RayConverter()
	{
		Type^ type = Ray::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("Position")),
			gcnew FieldPropertyDescriptor(type->GetField("Direction")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool RayConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool RayConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ RayConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		Ray^ ray = dynamic_cast<Ray^>( value );

		if( destinationType == String::typeid && ray != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ converter = TypeDescriptor::GetConverter(Vector3::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 2 );

			stringArray[0] = converter->ConvertToString( context, culture, ray->Position );
			stringArray[1] = converter->ConvertToString( context, culture, ray->Direction );

			return String::Join( separator, stringArray );
		}
		else if( destinationType == InstanceDescriptor::typeid && ray != nullptr )
		{
			ConstructorInfo^ info = (Ray::typeid)->GetConstructor( gcnew array<Type^> { Vector3::typeid, Vector3::typeid } );
			if( info != nullptr )
				return gcnew InstanceDescriptor( info, gcnew array<Object^> { ray->Position, ray->Direction } );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ RayConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
	{
		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		String^ string = dynamic_cast<String^>( value );

		if( string != nullptr )
		{
			string = string->Trim();
			TypeConverter^ converter = TypeDescriptor::GetConverter(Vector3::typeid);
			array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

			if( stringArray->Length != 2 )
				throw gcnew ArgumentException("Invalid ray format.");

			Vector3 Position = safe_cast<Vector3>( converter->ConvertFromString( context, culture, stringArray[0] ) );
			Vector3 Direction = safe_cast<Vector3>( converter->ConvertFromString( context, culture, stringArray[1] ) );

			return gcnew Ray(Position, Direction);
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool RayConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);

		return true;
	}

	Object^ RayConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		return gcnew Ray( safe_cast<Vector3>( propertyValues["Position"] ), safe_cast<Vector3>( propertyValues["Direction"] ) );
	}

	bool RayConverter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ RayConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}