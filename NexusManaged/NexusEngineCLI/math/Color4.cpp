#include "stdafx.h"
#include "Color4.h"
#include "Color3.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace System;
using namespace System::Globalization;
using namespace System::Drawing;

namespace NexusEngine
{
	/*Color4 Color4::FromUnmanaged( const D3DXCOLOR &color )
	{
		return Color4( color.a, color.r, color.g, color.b );
	}

	D3DXCOLOR Color4::ToUnmanaged()
	{
		D3DXCOLOR result( Red, Green, Blue, Alpha );
		return result;
	}*/

	Color4f::Color4f( float alpha, float red, float green, float blue )
	{
		A = alpha;
		R = red;
		G = green;
		B = blue;
	}

	Color4f::Color4f( float red, float green, float blue )
	{
		A = 1.0f;
		R = red;
		G = green;
		B = blue;
	}

	Color4f::Color4f( Color color )
	{
		A = color.A / 255.0f;
		R = color.R / 255.0f;
		G = color.G / 255.0f;
		B = color.B / 255.0f;
	}

	Color4f::Color4f( Color3 color )
	{
		A = 1.0f;
		R = color.R;
		G = color.G;
		B = color.B;
	}

	Color4f::Color4f( Vector3 color )
	{
		A = 1.0f;
		R = color.x;
		G = color.y;
		B = color.z;
	}

	Color4f::Color4f( Vector4 color )
	{
		A = color.w;
		R = color.x;
		G = color.y;
		B = color.z;
	}

	Color4f::Color4f( int argb )
	{
		A = ( ( argb >> 24 ) & 255 ) / 255.0f;
		R = ( ( argb >> 16 ) & 255 ) / 255.0f;
		G = ( ( argb >> 8 ) & 255 ) / 255.0f;
		B = ( argb & 255 ) / 255.0f;
	}

	Color Color4f::ToColor()
	{
		return Color::FromArgb( static_cast<int>(A * 255), static_cast<int>(R * 255), static_cast<int>(G * 255), static_cast<int>(B * 255) );
	}

	Color3 Color4f::ToColor3()
	{
		return Color3( R, G, B );
	}

	int Color4f::ToArgb()
	{
		unsigned int a, r, g, b;

		a = static_cast<unsigned int>(A * 255.0f);
		r = static_cast<unsigned int>(R * 255.0f);
		g = static_cast<unsigned int>(G * 255.0f);
		b = static_cast<unsigned int>(B * 255.0f);

		unsigned int value = b;
		value += g << 8;
		value += r << 16;
		value += a << 24;

		return static_cast<int>( value );
	}

	Vector3 Color4f::ToVector3()
	{
		return Vector3( R, G, B );
	}

	Vector4 Color4f::ToVector4()
	{
		return Vector4( R, G, B, A );
	}

	Color4f Color4f::Add( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A + color2.A, color1.R + color2.R, color1.G + color2.G, color1.B + color2.B );
	}

	void Color4f::Add( Color4f% color1, Color4f% color2, [Out] Color4f% result )
	{
		result = Color4f( color1.A + color2.A, color1.R + color2.R, color1.G + color2.G, color1.B + color2.B );
	}

	Color4f Color4f::Subtract( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A - color2.A, color1.R - color2.R, color1.G - color2.G, color1.B - color2.B );
	}

	void Color4f::Subtract( Color4f% color1, Color4f% color2, [Out] Color4f% result )
	{
		result = Color4f( color1.A - color2.A, color1.R - color2.R, color1.G - color2.G, color1.B - color2.B );
	}

	Color4f Color4f::Modulate( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A * color2.A, color1.R * color2.R, color1.G * color2.G, color1.B * color2.B );
	}

	void Color4f::Modulate( Color4f% color1, Color4f% color2, [Out] Color4f% result )
	{
		result = Color4f( color1.A * color2.A, color1.R * color2.R, color1.G * color2.G, color1.B * color2.B );
	}

	Color4f Color4f::Lerp( Color4f color1, Color4f color2, float amount )
	{
		float a = color1.A + amount * ( color2.A - color1.A );
		float r = color1.R + amount * ( color2.R - color1.R );
		float g = color1.G + amount * ( color2.G - color1.G );
		float b = color1.B + amount * ( color2.B - color1.B );

		return Color4f( a, r, g, b );
	}

	void Color4f::Lerp( Color4f% color1, Color4f% color2, float amount, [Out] Color4f% result )
	{
		float a = color1.A + amount * ( color2.A - color1.A );
		float r = color1.R + amount * ( color2.R - color1.R );
		float g = color1.G + amount * ( color2.G - color1.G );
		float b = color1.B + amount * ( color2.B - color1.B );

		result = Color4f( a, r, g, b );
	}

	Color4f Color4f::Negate( Color4f color )
	{
		return Color4f( 1.0f - color.A, 1.0f - color.R, 1.0f - color.G, 1.0f - color.B );
	}

	void Color4f::Negate( Color4f% color, [Out] Color4f% result )
	{
		result = Color4f( 1.0f - color.A, 1.0f - color.R, 1.0f - color.G, 1.0f - color.B );
	}

	Color4f Color4f::AdjustContrast( Color4f color, float contrast )
	{
		float r = 0.5f + contrast * ( color.R - 0.5f );
		float g = 0.5f + contrast * ( color.G - 0.5f );
		float b = 0.5f + contrast * ( color.B - 0.5f );

		return Color4f( color.A, r, g, b );
	}

	void Color4f::AdjustContrast( Color4f% color, float contrast, [Out] Color4f% result )
	{
		float r = 0.5f + contrast * ( color.R - 0.5f );
		float g = 0.5f + contrast * ( color.G - 0.5f );
		float b = 0.5f + contrast * ( color.B - 0.5f );

		result = Color4f( color.A, r, g, b );
	}

	Color4f Color4f::AdjustSaturation( Color4f color, float saturation )
	{
		float grey = color.R * 0.2125f + color.G * 0.7154f + color.B * 0.0721f;
		float r = grey + saturation * ( color.R - grey );
		float g = grey + saturation * ( color.G - grey );
		float b = grey + saturation * ( color.B - grey );

		return Color4f( color.A, r, g, b );
	}

	void Color4f::AdjustSaturation( Color4f% color, float saturation, [Out] Color4f% result )
	{
		float grey = color.R * 0.2125f + color.G * 0.7154f + color.B * 0.0721f;
		float r = grey + saturation * ( color.R - grey );
		float g = grey + saturation * ( color.G - grey );
		float b = grey + saturation * ( color.B - grey );

		result = Color4f( color.A, r, g, b );
	}

	Color4f Color4f::Scale( Color4f color, float scale )
	{
		return Color4f( color.A, color.R * scale, color.G * scale, color.B * scale );
	}

	void Color4f::Scale( Color4f% color, float scale, [Out] Color4f% result )
	{
		result = Color4f( color.A, color.R * scale, color.G * scale, color.B * scale );
	}

	Color4f Color4f::operator + ( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A + color2.A, color1.R + color2.R, color1.G + color2.G, color1.B + color2.B );
	}

	Color4f Color4f::operator - ( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A - color2.A, color1.R - color2.R, color1.G - color2.G, color1.B - color2.B );
	}

	Color4f Color4f::operator - ( Color4f color )
	{
		return Color4f( 1.0f - color.A, 1.0f - color.R, 1.0f - color.G, 1.0f - color.B );
	}

	Color4f Color4f::operator * ( Color4f color1, Color4f color2 )
	{
		return Color4f( color1.A * color2.A, color1.R * color2.R, color1.G * color2.G, color1.B * color2.B );
	}

	Color4f Color4f::operator * ( Color4f color, float scale )
	{
		return Color4f( color.A, color.R * scale, color.G * scale, color.B * scale );
	}

	Color4f Color4f::operator * ( float scale, Color4f value )
	{
		return value * scale;
	}

	bool Color4f::operator == ( Color4f left, Color4f right )
	{
		return Color4f::Equals( left, right );
	}

	bool Color4f::operator != ( Color4f left, Color4f right )
	{
		return !Color4f::Equals( left, right );
	}

	Color4f::operator int( Color4f value )
	{
		return value.ToArgb();
	}

	Color4f::operator Color3( Color4f value )
	{
		return value.ToColor3();
	}

	Color4f::operator System::Drawing::Color( Color4f value )
	{
		return value.ToColor();
	}

	Color4f::operator Vector3( Color4f value )
	{
		return value.ToVector3();
	}

	Color4f::operator Vector4( Color4f value )
	{
		return value.ToVector4();
	}

	Color4f::operator Color4f( int value )
	{
		return Color4f( value );
	}

	Color4f::operator Color4f( Color3 value )
	{
		return Color4f( value );
	}

	Color4f::operator Color4f( System::Drawing::Color value )
	{
		return Color4f( value );
	}

	Color4f::operator Color4f( Vector3 value )
	{
		return Color4f( value );
	}

	Color4f::operator Color4f( Vector4 value )
	{
		return Color4f( value );
	}

	String^ Color4f::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "A:{0} R:{1} G:{2} B:{3}", 
			A.ToString(CultureInfo::CurrentCulture), R.ToString(CultureInfo::CurrentCulture), 
			G.ToString(CultureInfo::CurrentCulture), B.ToString(CultureInfo::CurrentCulture) );
	}

	int Color4f::GetHashCode()
	{
		return A.GetHashCode() + R.GetHashCode() + G.GetHashCode() + B.GetHashCode();
	}

	bool Color4f::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Color4f>( value ) );
	}

	bool Color4f::Equals( Color4f value )
	{
		return ( A == value.A && R == value.R && G == value.G && B == value.B );
	}

	bool Color4f::Equals( Color4f% value1, Color4f% value2 )
	{
		return ( value1.A == value2.A && value1.R == value2.R && value1.G == value2.G && value1.B == value2.B );
	}
}