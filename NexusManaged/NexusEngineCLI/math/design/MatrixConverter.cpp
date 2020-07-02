#include "stdafx.h"
//#include "../InternalHelpers.h"
#include "../Matrix.h"
#include "MatrixConverter.h"
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
	MatrixConverter::MatrixConverter()
	{
		Type^ type = Matrix::typeid;
		array<PropertyDescriptor^>^ propArray =
		{
			gcnew FieldPropertyDescriptor(type->GetField("M11")),
			gcnew FieldPropertyDescriptor(type->GetField("M12")),
			gcnew FieldPropertyDescriptor(type->GetField("M13")),
			gcnew FieldPropertyDescriptor(type->GetField("M14")),

			gcnew FieldPropertyDescriptor(type->GetField("M21")),
			gcnew FieldPropertyDescriptor(type->GetField("M22")),
			gcnew FieldPropertyDescriptor(type->GetField("M23")),
			gcnew FieldPropertyDescriptor(type->GetField("M24")),

			gcnew FieldPropertyDescriptor(type->GetField("M31")),
			gcnew FieldPropertyDescriptor(type->GetField("M32")),
			gcnew FieldPropertyDescriptor(type->GetField("M33")),
			gcnew FieldPropertyDescriptor(type->GetField("M34")),

			gcnew FieldPropertyDescriptor(type->GetField("M41")),
			gcnew FieldPropertyDescriptor(type->GetField("M42")),
			gcnew FieldPropertyDescriptor(type->GetField("M43")),
			gcnew FieldPropertyDescriptor(type->GetField("M44")),
		};

		m_Properties = gcnew PropertyDescriptorCollection(propArray);
	}

	bool MatrixConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	bool MatrixConverter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
	{
		if( sourceType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertFrom(context, sourceType);
	}

	Object^ MatrixConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		Matrix^ matrix = dynamic_cast<Matrix^>( value );

		if( destinationType == String::typeid && matrix != nullptr )
		{
			String^ separator = culture->TextInfo->ListSeparator + " ";
			TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = gcnew array<String^>( 16 );

			for( int i = 0; i < 4; i++ )
			{
				for( int j = 0; j < 4; j++ )
					stringArray[i * 4 + j] = converter->ConvertToString( context, culture, matrix[i, j] );
			}

			return String::Join( separator, stringArray );
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}

	Object^ MatrixConverter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
	{
		if( culture == nullptr )
			culture = CultureInfo::CurrentCulture;

		String^ string = dynamic_cast<String^>( value );

		if( string != nullptr )
		{
			string = string->Trim();
			TypeConverter^ converter = TypeDescriptor::GetConverter(float::typeid);
			array<String^>^ stringArray = string->Split( culture->TextInfo->ListSeparator[0] );

			if( stringArray->Length != 16 )
				throw gcnew ArgumentException("Invalid matrix format.");

			Matrix matrix;

			for( int i = 0; i < 4; i++ )
			{
				for( int j = 0; j < 4; j++ )
					matrix[i, j] = safe_cast<float>( converter->ConvertFromString( context, culture, stringArray[i * 4 + j] ) );
			}

			return matrix;
		}

		return ExpandableObjectConverter::ConvertFrom(context, culture, value);
	}

	bool MatrixConverter::GetCreateInstanceSupported(ITypeDescriptorContext^ context)
	{
		UNREFERENCED_PARAMETER(context);

		return true;
	}

	Object^ MatrixConverter::CreateInstance(ITypeDescriptorContext^ context, IDictionary^ propertyValues)
	{
		UNREFERENCED_PARAMETER(context);

		if( propertyValues == nullptr )
			throw gcnew ArgumentNullException( "propertyValues" );

		Matrix matrix;

		matrix.M11 = safe_cast<float>( propertyValues["M11"] );
		matrix.M12 = safe_cast<float>( propertyValues["M12"] );
		matrix.M13 = safe_cast<float>( propertyValues["M13"] );
		matrix.M14 = safe_cast<float>( propertyValues["M14"] );

		matrix.M21 = safe_cast<float>( propertyValues["M21"] );
		matrix.M22 = safe_cast<float>( propertyValues["M22"] );
		matrix.M23 = safe_cast<float>( propertyValues["M23"] );
		matrix.M24 = safe_cast<float>( propertyValues["M24"] );

		matrix.M31 = safe_cast<float>( propertyValues["M31"] );
		matrix.M32 = safe_cast<float>( propertyValues["M32"] );
		matrix.M33 = safe_cast<float>( propertyValues["M33"] );
		matrix.M34 = safe_cast<float>( propertyValues["M34"] );

		matrix.M41 = safe_cast<float>( propertyValues["M41"] );
		matrix.M42 = safe_cast<float>( propertyValues["M42"] );
		matrix.M43 = safe_cast<float>( propertyValues["M43"] );
		matrix.M44 = safe_cast<float>( propertyValues["M44"] );

		return matrix;
	}

	bool MatrixConverter::GetPropertiesSupported(ITypeDescriptorContext^)
	{
		return true;
	}

	PropertyDescriptorCollection^ MatrixConverter::GetProperties(ITypeDescriptorContext^, Object^, array<Attribute^>^)
	{
		return m_Properties;
	}
}
}