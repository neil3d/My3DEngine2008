#include "stdafx.h"
#include "../BoundingSphere.h"
#include "BoundingSphereConverter.h"
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
	BoundingSphereConverter::BoundingSphereConverter()
	{
		Type^ type = BoundingSphere::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("Center")),
			gcnew FieldPropertyDescriptor(type->GetField("Radius")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool BoundingSphereConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool BoundingSphereConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ BoundingSphereConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		BoundingSphere^ sphere = dynamic_cast<BoundingSphere^>( value );

		if( destinationType == String::typeid && sphere != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ vector3Converter = TypeDescriptor::GetConverter(Vector3::typeid);
			TypeConverter^ floatConverter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 2 );

			stringArray[0] = vector3Converter->ConvertToString( context, culture, sphere->Center );
			stringArray[1] = floatConverter->ConvertToString( context, culture, sphere->Radius );

			return String::Join( separator, stringArray );
		}
		else if( destinationType == InstanceDescriptor::typeid && sphere != nullptr )
		{
			ConstructorInfo^ info = (BoundingSphere::typeid)->GetConstructor( gcnew array<Type^> { Vector3::typeid, float::typeid } );
			if( info != nullptr )
				return gcnew InstanceDescriptor( info, gcnew array<Object^> { sphere->Center, sphere->Radius } );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ BoundingSphereConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
	{
		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		String^ string = dynamic_cast<String^>( value );

		if( string != nullptr )
		{
			string = string->Trim();
			TypeConverter^ vector3Converter = TypeDescriptor::GetConverter(Vector3::typeid);
			TypeConverter^ floatConverter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

			if( stringArray->Length != 2 )
				throw gcnew ArgumentException("Invalid sphere format.");

			Vector3 Center = safe_cast<Vector3>( vector3Converter->ConvertFromString( context, culture, stringArray[0] ) );
			float Radius = safe_cast<float>( floatConverter->ConvertFromString( context, culture, stringArray[1] ) );

			return gcnew BoundingSphere(Center, Radius);
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool BoundingSphereConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);

		return true;
	}

	Object^ BoundingSphereConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		return gcnew BoundingSphere( safe_cast<Vector3>( propertyValues["Center"] ), safe_cast<float>( propertyValues["Radius"] ) );
	}

	bool BoundingSphereConverter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ BoundingSphereConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}