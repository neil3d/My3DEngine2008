#include "stdafx.h"
//#include "../InternalHelpers.h"
#include "../BoundingBox.h"
#include "BoundingBoxConverter.h"
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
	BoundingBoxConverter::BoundingBoxConverter()
	{
		Type^ type = BoundingBox::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("Minimum")),
			gcnew FieldPropertyDescriptor(type->GetField("Maximum")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool BoundingBoxConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid || destinationType == InstanceDescriptor::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool BoundingBoxConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ BoundingBoxConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		BoundingBox^ box = dynamic_cast<BoundingBox^>( value );

		if( destinationType == String::typeid && box != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ converter = TypeDescriptor::GetConverter(Vector3::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 2 );

			stringArray[0] = converter->ConvertToString( context, culture, box->Maximum );
			stringArray[1] = converter->ConvertToString( context, culture, box->Minimum );

			return String::Join( separator, stringArray );
		}
		else if( destinationType == InstanceDescriptor::typeid && box != nullptr )
		{
			ConstructorInfo^ info = (BoundingBox::typeid)->GetConstructor( gcnew array<Type^> { Vector3::typeid, Vector3::typeid } );
			if( info != nullptr )
				return gcnew InstanceDescriptor( info, gcnew array<Object^> { box->Maximum, box->Minimum } );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ BoundingBoxConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
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
				throw gcnew ArgumentException("Invalid box format.");

			Vector3 Maximum = safe_cast<Vector3>( converter->ConvertFromString( context, culture, stringArray[0] ) );
			Vector3 Minimum = safe_cast<Vector3>( converter->ConvertFromString( context, culture, stringArray[1] ) );

			return gcnew BoundingBox(Maximum, Minimum);
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool BoundingBoxConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);
		return true;
	}

	Object^ BoundingBoxConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		return gcnew BoundingBox( safe_cast<Vector3>( propertyValues["Maximum"] ), safe_cast<Vector3>( propertyValues["Minimum"] ) );
	}

	bool BoundingBoxConverter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ BoundingBoxConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}