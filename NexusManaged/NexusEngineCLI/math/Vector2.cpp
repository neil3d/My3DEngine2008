#include "stdafx.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vector2.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Vector2::Vector2( float value )
	{
		x = value;
		y = value;
	}
	
	Vector2::Vector2( float _x, float _y )
	{
		x = _x;
		y = _y;
	}
	
	float Vector2::default::get( int index )
	{
		switch( index )
		{
		case 0:
			return x;

		case 1:
			return y;

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector2 run from 0 to 1, inclusive." );
		}
	}
	
	void Vector2::default::set( int index, float value )
	{
		switch( index )
		{
		case 0:
			x = value;
			break;

		case 1:
			y = value;
			break;

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector2 run from 0 to 1, inclusive." );
		}
	}
	
	float Vector2::Length()
	{
		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) ) );
	}
	
	float Vector2::LengthSquared()
	{
		return (x * x) + (y * y);
	}
	
	void Vector2::Normalize()
	{
		float length = Length();
		if( length == 0 )
			return;
		float num = 1 / length;
		x *= num;
		y *= num;
	}

	Vector2 Vector2::Add( Vector2 left, Vector2 right )
	{
		return Vector2( left.x + right.x, left.y + right.y );
	}
	
	void Vector2::Add( Vector2% left, Vector2% right, [Out] Vector2% result )
	{
		result = Vector2( left.x + right.x, left.y + right.y );
	}
	
	Vector2 Vector2::Subtract( Vector2 left, Vector2 right )
	{
		return Vector2( left.x - right.x, left.y - right.y );
	}
	
	void Vector2::Subtract( Vector2% left, Vector2% right, [Out] Vector2% result )
	{
		result = Vector2( left.x - right.x, left.y - right.y );
	}
	
	Vector2 Vector2::Modulate( Vector2 left, Vector2 right )
	{
		return Vector2( left.x * right.x, left.y * right.y );
	}
	
	void Vector2::Modulate( Vector2% left, Vector2% right, [Out] Vector2% result )
	{
		result = Vector2( left.x * right.x, left.y * right.y );
	}
	
	Vector2 Vector2::Multiply( Vector2 value, float scale )
	{
		return Vector2( value.x * scale, value.y * scale );
	}
	
	void Vector2::Multiply( Vector2% value, float scale, [Out] Vector2% result )
	{
		result = Vector2( value.x * scale, value.y * scale );
	}
	
	Vector2 Vector2::Divide( Vector2 value, float scale )
	{
		return Vector2( value.x / scale, value.y / scale );
	}

	void Vector2::Divide( Vector2% value, float scale, [Out] Vector2% result )
	{
		result = Vector2( value.x / scale, value.y / scale );
	}

	Vector2 Vector2::Negate( Vector2 value )
	{
		return Vector2( -value.x, -value.y );
	}
	
	void Vector2::Negate( Vector2% value, [Out] Vector2% result )
	{
		result = Vector2( -value.x, -value.y );
	}
	
	Vector2 Vector2::Barycentric( Vector2 value1, Vector2 value2, Vector2 value3, float amount1, float amount2 )
	{
		Vector2 vector;
		vector.x = (value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x));
		vector.y = (value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y));
		return vector;
	}
	
	void Vector2::Barycentric( Vector2% value1, Vector2% value2, Vector2% value3, float amount1, float amount2, [Out] Vector2% result )
	{
		result = Vector2((value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x)),
			(value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y)) );
	}
	
	Vector2 Vector2::CatmullRom( Vector2 value1, Vector2 value2, Vector2 value3, Vector2 value4, float amount )
	{
		Vector2 vector;
		float squared = amount * amount;
		float cubed = amount * squared;

		vector.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		vector.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		return vector;
	}
	
	void Vector2::CatmullRom( Vector2% value1, Vector2% value2, Vector2% value3, Vector2% value4, float amount, [Out] Vector2% result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		Vector2 r;

		r.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		r.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		result = r;
	}
	
	Vector2 Vector2::Clamp( Vector2 value, Vector2 min, Vector2 max )
	{
		float x = value.x;
		x = (x > max.x) ? max.x : x;
		x = (x < min.x) ? min.x : x;

		float y = value.y;
		y = (y > max.y) ? max.y : y;
		y = (y < min.y) ? min.y : y;

		return Vector2( x, y );
	}
	
	void Vector2::Clamp( Vector2% value, Vector2% min, Vector2% max, [Out] Vector2% result )
	{
		float x = value.x;
		x = (x > max.x) ? max.x : x;
		x = (x < min.x) ? min.x : x;

		float y = value.y;
		y = (y > max.y) ? max.y : y;
		y = (y < min.y) ? min.y : y;

		result = Vector2( x, y );
	}
	
	Vector2 Vector2::Hermite( Vector2 value1, Vector2 tangent1, Vector2 value2, Vector2 tangent2, float amount )
	{
		Vector2 vector;
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		vector.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		vector.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);

		return vector;
	}	
	
	void Vector2::Hermite( Vector2% value1, Vector2% tangent1, Vector2% value2, Vector2% tangent2, float amount, [Out] Vector2% result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		Vector2 r;
		r.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		r.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);

		result = r;
	}
	
	Vector2 Vector2::Lerp( Vector2 start, Vector2 end, float factor )
	{
		Vector2 vector;

		vector.x = start.x + ((end.x - start.x) * factor);
		vector.y = start.y + ((end.y - start.y) * factor);

		return vector;
	}
	
	void Vector2::Lerp( Vector2% start, Vector2% end, float factor, [Out] Vector2% result )
	{
		Vector2 r;
		r.x = start.x + ((end.x - start.x) * factor);
		r.y = start.y + ((end.y - start.y) * factor);

		result = r;
	}
	
	Vector2 Vector2::SmoothStep( Vector2 start, Vector2 end, float amount )
	{
		Vector2 vector;

		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		vector.x = start.x + ((end.x - start.x) * amount);
		vector.y = start.y + ((end.y - start.y) * amount);

		return vector;
	}
	
	void Vector2::SmoothStep( Vector2% start, Vector2% end, float amount, [Out] Vector2% result )
	{
		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		Vector2 r;
		r.x = start.x + ((end.x - start.x) * amount);
		r.y = start.y + ((end.y - start.y) * amount);

		result = r;
	}
	
	float Vector2::Distance( Vector2 value1, Vector2 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;

		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) ) );
	}

	float Vector2::DistanceSquared( Vector2 value1, Vector2 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;

		return (x * x) + (y * y);
	}
	
	float Vector2::Dot( Vector2 left, Vector2 right )
	{
		return (left.x * right.x + left.y * right.y);
	}
	
	Vector2 Vector2::Normalize( Vector2 vector )
	{
		vector.Normalize();
		return vector;
	}
	
	void Vector2::Normalize( Vector2% vector, [Out] Vector2% result )
	{
		result = Vector2::Normalize(vector);
	}
	
	Vector4 Vector2::Transform( Vector2 vector, Matrix transform )
	{
		Vector4 result;

		result.x = (vector.x * transform.M11) + (vector.y * transform.M21) + transform.M41;
		result.y = (vector.x * transform.M12) + (vector.y * transform.M22) + transform.M42;
		result.z = (vector.x * transform.M13) + (vector.y * transform.M23) + transform.M43;
		result.w = (vector.x * transform.M14) + (vector.y * transform.M24) + transform.M44;

		return result;
	}
	
	void Vector2::Transform( Vector2% vector, Matrix% transform, [Out] Vector4% result )
	{
		Vector4 r;
		r.x = (vector.x * transform.M11) + (vector.y * transform.M21) + transform.M41;
		r.y = (vector.x * transform.M12) + (vector.y * transform.M22) + transform.M42;
		r.z = (vector.x * transform.M13) + (vector.y * transform.M23) + transform.M43;
		r.w = (vector.x * transform.M14) + (vector.y * transform.M24) + transform.M44;

		result = r;
	}
	
	array<Vector4>^ Vector2::Transform( array<Vector2>^ vectors, Matrix% transform )
	{
		if( vectors == nullptr )
			throw gcnew ArgumentNullException( "vectors" );

		int count = vectors->Length;
		array<Vector4>^ results = gcnew array<Vector4>( count );

		for( int i = 0; i < count; i++ )
		{
			Vector4 r;
			r.x = (vectors[i].x * transform.M11) + (vectors[i].y * transform.M21) + transform.M41;
			r.y = (vectors[i].x * transform.M12) + (vectors[i].y * transform.M22) + transform.M42;
			r.z = (vectors[i].x * transform.M13) + (vectors[i].y * transform.M23) + transform.M43;
			r.w = (vectors[i].x * transform.M14) + (vectors[i].y * transform.M24) + transform.M44;

			results[i] = r;
		}

		return results;
	}
	
	Vector4 Vector2::Transform( Vector2 value, Quaternion rotation )
	{
		Vector4 vector;
		float x = rotation.x + rotation.x;
		float y = rotation.y + rotation.y;
		float z = rotation.z + rotation.z;
		float wx = rotation.w * x;
		float wy = rotation.w * y;
		float wz = rotation.w * z;
		float xx = rotation.x * x;
		float xy = rotation.x * y;
		float xz = rotation.x * z;
		float yy = rotation.y * y;
		float yz = rotation.y * z;
		float zz = rotation.z * z;

		vector.x = ((value.x * ((1.0f - yy) - zz)) + (value.y * (xy - wz)));
		vector.y = ((value.x * (xy + wz)) + (value.y * ((1.0f - xx) - zz)));
		vector.z = ((value.x * (xz - wy)) + (value.y * (yz + wx)));
		vector.w = 1.0f;

		return vector;
	}
	
	void Vector2::Transform( Vector2% value, Quaternion% rotation, [Out] Vector4% result )
	{
		float x = rotation.x + rotation.x;
		float y = rotation.y + rotation.y;
		float z = rotation.z + rotation.z;
		float wx = rotation.w * x;
		float wy = rotation.w * y;
		float wz = rotation.w * z;
		float xx = rotation.x * x;
		float xy = rotation.x * y;
		float xz = rotation.x * z;
		float yy = rotation.y * y;
		float yz = rotation.y * z;
		float zz = rotation.z * z;

		Vector4 r;
		r.x = ((value.x * ((1.0f - yy) - zz)) + (value.y * (xy - wz)));
		r.y = ((value.x * (xy + wz)) + (value.y * ((1.0f - xx) - zz)));
		r.z = ((value.x * (xz - wy)) + (value.y * (yz + wx)));
		r.w = 1.0f;

		result = r;
	}

	array<Vector4>^ Vector2::Transform( array<Vector2>^ vectors, Quaternion% rotation )
	{
		if( vectors == nullptr )
			throw gcnew ArgumentNullException( "vectors" );

		int count = vectors->Length;
		array<Vector4>^ results = gcnew array<Vector4>( count );

		float x = rotation.x + rotation.x;
		float y = rotation.y + rotation.y;
		float z = rotation.z + rotation.z;
		float wx = rotation.w * x;
		float wy = rotation.w * y;
		float wz = rotation.w * z;
		float xx = rotation.x * x;
		float xy = rotation.x * y;
		float xz = rotation.x * z;
		float yy = rotation.y * y;
		float yz = rotation.y * z;
		float zz = rotation.z * z;

		for( int i = 0; i < count; i++ )
		{
			Vector4 r;
			r.x = ((vectors[i].x * ((1.0f - yy) - zz)) + (vectors[i].y * (xy - wz)));
			r.y = ((vectors[i].x * (xy + wz)) + (vectors[i].y * ((1.0f - xx) - zz)));
			r.z = ((vectors[i].x * (xz - wy)) + (vectors[i].y * (yz + wx)));
			r.w = 1.0f;

			results[i] = r;
		}

		return results;
	}
	
	Vector2 Vector2::TransformCoordinate( Vector2 coord, Matrix transform )
	{
		Vector4 vector;

		vector.x = (coord.x * transform.M11) + (coord.y * transform.M21) + transform.M41;
		vector.y = (coord.x * transform.M12) + (coord.y * transform.M22) + transform.M42;
		vector.z = (coord.x * transform.M13) + (coord.y * transform.M23) + transform.M43;
		vector.w = 1 / ((coord.x * transform.M14) + (coord.y * transform.M24) + transform.M44);

		return Vector2( vector.x * vector.w, vector.y * vector.w );
	}
	
	void Vector2::TransformCoordinate( Vector2% coord, Matrix% transform, [Out] Vector2% result )
	{
		Vector4 vector;

		vector.x = (coord.x * transform.M11) + (coord.y * transform.M21) + transform.M41;
		vector.y = (coord.x * transform.M12) + (coord.y * transform.M22) + transform.M42;
		vector.z = (coord.x * transform.M13) + (coord.y * transform.M23) + transform.M43;
		vector.w = 1 / ((coord.x * transform.M14) + (coord.y * transform.M24) + transform.M44);

		result = Vector2( vector.x * vector.w, vector.y * vector.w );
	}
	
	array<Vector2>^ Vector2::TransformCoordinate( array<Vector2>^ coords, Matrix% transform )
	{
		if( coords == nullptr )
			throw gcnew ArgumentNullException( "coordinates" );

		Vector4 vector;
		int count = coords->Length;
		array<Vector2>^ results = gcnew array<Vector2>( count );

		for( int i = 0; i < count; i++ )
		{
			vector.x = (coords[i].x * transform.M11) + (coords[i].y * transform.M21) + transform.M41;
			vector.y = (coords[i].x * transform.M12) + (coords[i].y * transform.M22) + transform.M42;
			vector.z = (coords[i].x * transform.M13) + (coords[i].y * transform.M23) + transform.M43;
			vector.w = 1 / ((coords[i].x * transform.M14) + (coords[i].y * transform.M24) + transform.M44);
			results[i] = Vector2( vector.x * vector.w, vector.y * vector.w );
		}

		return results;
	}

	Vector2 Vector2::TransformNormal( Vector2 normal, Matrix transform )
	{
		Vector2 vector;

		vector.x = (normal.x * transform.M11) + (normal.y * transform.M21);
		vector.y = (normal.x * transform.M12) + (normal.y * transform.M22);

		return vector;
	}
	
	void Vector2::TransformNormal( Vector2% normal, Matrix% transform, [Out] Vector2% result )
	{
		Vector2 r;
		r.x = (normal.x * transform.M11) + (normal.y * transform.M21);
		r.y = (normal.x * transform.M12) + (normal.y * transform.M22);

		result = r;
	}
	
	array<Vector2>^ Vector2::TransformNormal( array<Vector2>^ normals, Matrix% transform )
	{
		if( normals == nullptr )
			throw gcnew ArgumentNullException( "normals" );

		int count = normals->Length;
		array<Vector2>^ results = gcnew array<Vector2>( count );

		for( int i = 0; i < count; i++ )
		{
			Vector2 r;
			r.x = (normals[i].x * transform.M11) + (normals[i].y * transform.M21);
			r.y = (normals[i].x * transform.M12) + (normals[i].y * transform.M22);

			results[i] = r;
		}

		return results;
	}
	
	Vector2 Vector2::Minimize( Vector2 left, Vector2 right )
	{
		Vector2 vector;
		vector.x = (left.x < right.x) ? left.x : right.x;
		vector.y = (left.y < right.y) ? left.y : right.y;
		return vector;
	}
	
	void Vector2::Minimize( Vector2% left, Vector2% right, [Out] Vector2% result )
	{
		Vector2 r;
		r.x = (left.x < right.x) ? left.x : right.x;
		r.y = (left.y < right.y) ? left.y : right.y;

		result = r;
	}
	
	Vector2 Vector2::Maximize( Vector2 left, Vector2 right )
	{
		Vector2 vector;
		vector.x = (left.x > right.x) ? left.x : right.x;
		vector.y = (left.y > right.y) ? left.y : right.y;
		return vector;
	}

	void Vector2::Maximize( Vector2% left, Vector2% right, [Out] Vector2% result )
	{
		Vector2 r;
		r.x = (left.x > right.x) ? left.x : right.x;
		r.y = (left.y > right.y) ? left.y : right.y;

		result = r;
	}
	
	Vector2 Vector2::operator + ( Vector2 left, Vector2 right )
	{
		return Vector2( left.x + right.x, left.y + right.y );
	}

	Vector2 Vector2::operator - ( Vector2 left, Vector2 right )
	{
		return Vector2( left.x - right.x, left.y - right.y );
	}
	
	Vector2 Vector2::operator - ( Vector2 value )
	{
		return Vector2( -value.x, -value.y );
	}
	
	Vector2 Vector2::operator * ( Vector2 value, float scale )
	{
		return Vector2( value.x * scale, value.y * scale );
	}
	
	Vector2 Vector2::operator * ( float scale, Vector2 vec )
	{
		return vec * scale;
	}
	
	Vector2 Vector2::operator / ( Vector2 value, float scale )
	{
		return Vector2( value.x / scale, value.y / scale );
	}
	
	bool Vector2::operator == ( Vector2 left, Vector2 right )
	{
		return Vector2::Equals( left, right );
	}
	
	bool Vector2::operator != ( Vector2 left, Vector2 right )
	{
		return !Vector2::Equals( left, right );
	}
	
	String^ Vector2::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "X:{0} Y:{1}", x.ToString(CultureInfo::CurrentCulture), y.ToString(CultureInfo::CurrentCulture) );
	}

	int Vector2::GetHashCode()
	{
		return x.GetHashCode() + y.GetHashCode();
	}

	bool Vector2::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Vector2>( value ) );
	}

	bool Vector2::Equals( Vector2 value )
	{
		return ( x == value.x && y == value.y );
	}

	bool Vector2::Equals( Vector2% value1, Vector2% value2 )
	{
		return ( value1.x == value2.x && value1.y == value2.y );
	}
}