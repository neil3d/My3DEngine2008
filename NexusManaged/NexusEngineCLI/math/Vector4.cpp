#include "stdafx.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Vector4::Vector4( float value )
	{
		x = value;
		y = value;
		z = value;
		w = value;
	}
	
	Vector4::Vector4( Vector2 value, float _z, float _w )
	{
		x = value.x;
		y = value.y;
		z = _z;
		w = _w;
	}
	
	Vector4::Vector4( Vector3 value, float _w )
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = _w;
	}
	
	Vector4::Vector4( float _x, float _y, float _z, float _w )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	float Vector4::default::get( int index )
	{
		switch( index )
		{
		case 0:
			return x;

		case 1:
			return y;

		case 2:
			return z;

		case 3:
			return w;

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector4 run from 0 to 3, inclusive." );
		}
	}
	
	void Vector4::default::set( int index, float value )
	{
		switch( index )
		{
		case 0:
			x = value;
			break;

		case 1:
			y = value;
			break;

		case 2:
			z = value;
			break;

		case 3:
			w = value;
			break;

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector4 run from 0 to 3, inclusive." );
		}
	}
	
	float Vector4::Length()
	{
		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) + (w * w) ) );
	}
	
	float Vector4::LengthSquared()
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}
	
	void Vector4::Normalize()
	{
		float length = Length();
		if( length == 0 )
			return;
		float num = 1 / length;
		x *= num;
		y *= num;
		z *= num;
		w *= num;
	}
	
	Vector4 Vector4::Add( Vector4 left, Vector4 right )
	{
		return Vector4( left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w );
	}
	
	void Vector4::Add( Vector4% left, Vector4% right, [Out] Vector4% result )
	{
		result = Vector4( left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w );
	}
	
	Vector4 Vector4::Subtract( Vector4 left, Vector4 right )
	{
		return Vector4( left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w );
	}
	
	void Vector4::Subtract( Vector4% left, Vector4% right, [Out] Vector4% result )
	{
		result = Vector4( left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w );
	}
	
	Vector4 Vector4::Modulate( Vector4 left, Vector4 right )
	{
		return Vector4( left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w );
	}
	
	void Vector4::Modulate( Vector4% left, Vector4% right, [Out] Vector4% result )
	{
		result = Vector4( left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w );
	}
	
	Vector4 Vector4::Multiply( Vector4 value, float scale )
	{
		return Vector4( value.x * scale, value.y * scale, value.z * scale, value.w * scale );
	}
	
	void Vector4::Multiply( Vector4% value, float scale, [Out] Vector4% result )
	{
		result = Vector4( value.x * scale, value.y * scale, value.z * scale, value.w * scale );
	}
	
	Vector4 Vector4::Divide( Vector4 value, float scale )
	{
		return Vector4( value.x / scale, value.y / scale, value.z / scale, value.w / scale );
	}
	
	void Vector4::Divide( Vector4% value, float scale, [Out] Vector4% result )
	{
		result = Vector4( value.x / scale, value.y / scale, value.z / scale, value.w / scale );
	}
	
	Vector4 Vector4::Negate( Vector4 value )
	{
		return Vector4( -value.x, -value.y, -value.z, -value.w );
	}
	
	void Vector4::Negate( Vector4% value, [Out] Vector4% result )
	{
		result = Vector4( -value.x, -value.y, -value.z, -value.w );
	}
	
	Vector4 Vector4::Barycentric( Vector4 value1, Vector4 value2, Vector4 value3, float amount1, float amount2 )
	{
		Vector4 vector;
		vector.x = (value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x));
		vector.y = (value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y));
		vector.z = (value1.z + (amount1 * (value2.z - value1.z))) + (amount2 * (value3.z - value1.z));
		vector.w = (value1.w + (amount1 * (value2.w - value1.w))) + (amount2 * (value3.w - value1.w));
		return vector;
	}
	
	void Vector4::Barycentric( Vector4% value1, Vector4% value2, Vector4% value3, float amount1, float amount2, [Out] Vector4% result )
	{
		result = Vector4((value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x)),
			(value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y)),
			(value1.z + (amount1 * (value2.z - value1.z))) + (amount2 * (value3.z - value1.z)),
			(value1.w + (amount1 * (value2.w - value1.w))) + (amount2 * (value3.w - value1.w)) );
	}
	
	Vector4 Vector4::CatmullRom( Vector4 value1, Vector4 value2, Vector4 value3, Vector4 value4, float amount )
	{
		Vector4 vector;
		float squared = amount * amount;
		float cubed = amount * squared;

		vector.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		vector.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		vector.z = 0.5f * ((((2.0f * value2.z) + ((-value1.z + value3.z) * amount)) + 
			(((((2.0f * value1.z) - (5.0f * value2.z)) + (4.0f * value3.z)) - value4.z) * squared)) + 
			((((-value1.z + (3.0f * value2.z)) - (3.0f * value3.z)) + value4.z) * cubed));

		vector.w = 0.5f * ((((2.0f * value2.w) + ((-value1.w + value3.w) * amount)) + 
			(((((2.0f * value1.w) - (5.0f * value2.w)) + (4.0f * value3.w)) - value4.w) * squared)) + 
			((((-value1.w + (3.0f * value2.w)) - (3.0f * value3.w)) + value4.w) * cubed));

		return vector;
	}
	
	void Vector4::CatmullRom( Vector4% value1, Vector4% value2, Vector4% value3, Vector4% value4, float amount, [Out] Vector4% result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		
		Vector4 r;

		r.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		r.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		r.z = 0.5f * ((((2.0f * value2.z) + ((-value1.z + value3.z) * amount)) + 
			(((((2.0f * value1.z) - (5.0f * value2.z)) + (4.0f * value3.z)) - value4.z) * squared)) + 
			((((-value1.z + (3.0f * value2.z)) - (3.0f * value3.z)) + value4.z) * cubed));

		r.w = 0.5f * ((((2.0f * value2.w) + ((-value1.w + value3.w) * amount)) + 
			(((((2.0f * value1.w) - (5.0f * value2.w)) + (4.0f * value3.w)) - value4.w) * squared)) + 
			((((-value1.w + (3.0f * value2.w)) - (3.0f * value3.w)) + value4.w) * cubed));

		result = r;
	}
	
	Vector4 Vector4::Clamp( Vector4 value, Vector4 min, Vector4 max )
	{
		float x = value.x;
		x = (x > max.x) ? max.x : x;
		x = (x < min.x) ? min.x : x;

		float y = value.y;
		y = (y > max.y) ? max.y : y;
		y = (y < min.y) ? min.y : y;

		float z = value.z;
		z = (z > max.z) ? max.z : z;
		z = (z < min.z) ? min.z : z;

		float w = value.w;
		w = (w > max.w) ? max.w : w;
		w = (w < min.w) ? min.w : w;

		return Vector4( x, y, z, w );
	}
	
	void Vector4::Clamp( Vector4% value, Vector4% min, Vector4% max, [Out] Vector4% result )
	{
		float x = value.x;
		x = (x > max.x) ? max.x : x;
		x = (x < min.x) ? min.x : x;

		float y = value.y;
		y = (y > max.y) ? max.y : y;
		y = (y < min.y) ? min.y : y;

		float z = value.z;
		z = (z > max.z) ? max.z : z;
		z = (z < min.z) ? min.z : z;

		float w = value.w;
		w = (w > max.w) ? max.w : w;
		w = (w < min.w) ? min.w : w;

		result = Vector4( x, y, z, w );
	}
	
	Vector4 Vector4::Hermite( Vector4 value1, Vector4 tangent1, Vector4 value2, Vector4 tangent2, float amount )
	{
		Vector4 vector;
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		vector.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		vector.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);
		vector.z = (((value1.z * part1) + (value2.z * part2)) + (tangent1.z * part3)) + (tangent2.z * part4);
		vector.w = (((value1.w * part1) + (value2.w * part2)) + (tangent1.w * part3)) + (tangent2.w * part4);

		return vector;
	}	
	
	void Vector4::Hermite( Vector4% value1, Vector4% tangent1, Vector4% value2, Vector4% tangent2, float amount, [Out] Vector4% result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		result.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		result.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);
		result.z = (((value1.z * part1) + (value2.z * part2)) + (tangent1.z * part3)) + (tangent2.z * part4);
		result.w = (((value1.w * part1) + (value2.w * part2)) + (tangent1.w * part3)) + (tangent2.w * part4);
	}
	
	Vector4 Vector4::Lerp( Vector4 start, Vector4 end, float factor )
	{
		Vector4 vector;

		vector.x = start.x + ((end.x - start.x) * factor);
		vector.y = start.y + ((end.y - start.y) * factor);
		vector.z = start.z + ((end.z - start.z) * factor);
		vector.w = start.w + ((end.w - start.w) * factor);

		return vector;
	}
	
	void Vector4::Lerp( Vector4% start, Vector4% end, float factor, [Out] Vector4% result )
	{
		result.x = start.x + ((end.x - start.x) * factor);
		result.y = start.y + ((end.y - start.y) * factor);
		result.z = start.z + ((end.z - start.z) * factor);
		result.w = start.w + ((end.w - start.w) * factor);
	}
	
	Vector4 Vector4::SmoothStep( Vector4 start, Vector4 end, float amount )
	{
		Vector4 vector;

		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		vector.x = start.x + ((end.x - start.x) * amount);
		vector.y = start.y + ((end.y - start.y) * amount);
		vector.z = start.z + ((end.z - start.z) * amount);
		vector.w = start.w + ((end.w - start.w) * amount);

		return vector;
	}
	
	void Vector4::SmoothStep( Vector4% start, Vector4% end, float amount, [Out] Vector4% result )
	{
		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		result.x = start.x + ((end.x - start.x) * amount);
		result.y = start.y + ((end.y - start.y) * amount);
		result.z = start.z + ((end.z - start.z) * amount);
		result.w = start.w + ((end.w - start.w) * amount);
	}
	
	float Vector4::Distance( Vector4 value1, Vector4 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;
		float z = value1.z - value2.z;
		float w = value1.w - value2.w;

		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) + (w * w) ) );
	}
	
	float Vector4::DistanceSquared( Vector4 value1, Vector4 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;
		float z = value1.z - value2.z;
		float w = value1.w - value2.w;

		return (x * x) + (y * y) + (z * z) + (w * w);
	}
	
	float Vector4::Dot( Vector4 left, Vector4 right )
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w);
	}

	Vector4 Vector4::Normalize( Vector4 vector )
	{
		vector.Normalize();
		return vector;
	}
	
	void Vector4::Normalize( Vector4% vector, [Out] Vector4% result )
	{
		result = Vector4(vector);
		result.Normalize();
	}
	
	Vector4 Vector4::Transform( Vector4 vector, Matrix transform )
	{
		Vector4 result;

		result.x = (vector.x * transform.M11) + (vector.y * transform.M21) + (vector.z * transform.M31) + (vector.w * transform.M41);
		result.y = (vector.x * transform.M12) + (vector.y * transform.M22) + (vector.z * transform.M32) + (vector.w * transform.M42);
		result.z = (vector.x * transform.M13) + (vector.y * transform.M23) + (vector.z * transform.M33) + (vector.w * transform.M43);
		result.w = (vector.x * transform.M14) + (vector.y * transform.M24) + (vector.z * transform.M34) + (vector.w * transform.M44);

		return result;
	}
	
	void Vector4::Transform( Vector4% vector, Matrix% transform, [Out] Vector4% result )
	{
		Vector4 r;
		r.x = (vector.x * transform.M11) + (vector.y * transform.M21) + (vector.z * transform.M31) + (vector.w * transform.M41);
		r.y = (vector.x * transform.M12) + (vector.y * transform.M22) + (vector.z * transform.M32) + (vector.w * transform.M42);
		r.z = (vector.x * transform.M13) + (vector.y * transform.M23) + (vector.z * transform.M33) + (vector.w * transform.M43);
		r.w = (vector.x * transform.M14) + (vector.y * transform.M24) + (vector.z * transform.M34) + (vector.w * transform.M44);
	
		result = r;
	}
	
	array<Vector4>^ Vector4::Transform( array<Vector4>^ vectors, Matrix% transform )
	{
		if( vectors == nullptr )
			throw gcnew ArgumentNullException( "vectors" );

		int count = vectors->Length;
		array<Vector4>^ results = gcnew array<Vector4>( count );

		for( int i = 0; i < count; i++ )
		{
			Vector4 r;
			r.x = (vectors[i].x * transform.M11) + (vectors[i].y * transform.M21) + (vectors[i].z * transform.M31) + (vectors[i].w * transform.M41);
			r.y = (vectors[i].x * transform.M12) + (vectors[i].y * transform.M22) + (vectors[i].z * transform.M32) + (vectors[i].w * transform.M42);
			r.z = (vectors[i].x * transform.M13) + (vectors[i].y * transform.M23) + (vectors[i].z * transform.M33) + (vectors[i].w * transform.M43);
			r.w = (vectors[i].x * transform.M14) + (vectors[i].y * transform.M24) + (vectors[i].z * transform.M34) + (vectors[i].w * transform.M44);
		
			results[i] = r;
		}

		return results;
	}
	
	Vector4 Vector4::Transform( Vector4 value, Quaternion rotation )
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

		vector.x = ((value.x * ((1.0f - yy) - zz)) + (value.y * (xy - wz))) + (value.z * (xz + wy));
		vector.y = ((value.x * (xy + wz)) + (value.y * ((1.0f - xx) - zz))) + (value.z * (yz - wx));
		vector.z = ((value.x * (xz - wy)) + (value.y * (yz + wx))) + (value.z * ((1.0f - xx) - yy));
		vector.w = value.w;

		return vector;
	}
	
	void Vector4::Transform( Vector4% value, Quaternion% rotation, [Out] Vector4% result )
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
		r.x = ((value.x * ((1.0f - yy) - zz)) + (value.y * (xy - wz))) + (value.z * (xz + wy));
		r.y = ((value.x * (xy + wz)) + (value.y * ((1.0f - xx) - zz))) + (value.z * (yz - wx));
		r.z = ((value.x * (xz - wy)) + (value.y * (yz + wx))) + (value.z * ((1.0f - xx) - yy));
		r.w = value.w;

		result = r;
	}
	
	array<Vector4>^ Vector4::Transform( array<Vector4>^ vectors, Quaternion% rotation )
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
			r.x = ((vectors[i].x * ((1.0f - yy) - zz)) + (vectors[i].y * (xy - wz))) + (vectors[i].z * (xz + wy));
			r.y = ((vectors[i].x * (xy + wz)) + (vectors[i].y * ((1.0f - xx) - zz))) + (vectors[i].z * (yz - wx));
			r.z = ((vectors[i].x * (xz - wy)) + (vectors[i].y * (yz + wx))) + (vectors[i].z * ((1.0f - xx) - yy));
			r.w = vectors[i].w;

			results[i] = r;
		}

		return results;
	}

	Vector4 Vector4::Minimize( Vector4 left, Vector4 right )
	{
		Vector4 vector;
		vector.x = (left.x < right.x) ? left.x : right.x;
		vector.y = (left.y < right.y) ? left.y : right.y;
		vector.z = (left.z < right.z) ? left.z : right.z;
		vector.w = (left.w < right.w) ? left.w : right.w;
		return vector;
	}
	
	void Vector4::Minimize( Vector4% left, Vector4% right, [Out] Vector4% result )
	{
		result.x = (left.x < right.x) ? left.x : right.x;
		result.y = (left.y < right.y) ? left.y : right.y;
		result.z = (left.z < right.z) ? left.z : right.z;
		result.w = (left.w < right.w) ? left.w : right.w;
	}

	Vector4 Vector4::Maximize( Vector4 left, Vector4 right )
	{
		Vector4 vector;
		vector.x = (left.x > right.x) ? left.x : right.x;
		vector.y = (left.y > right.y) ? left.y : right.y;
		vector.z = (left.z > right.z) ? left.z : right.z;
		vector.w = (left.w > right.w) ? left.w : right.w;
		return vector;
	}
	
	void Vector4::Maximize( Vector4% left, Vector4% right, [Out] Vector4% result )
	{
		result.x = (left.x > right.x) ? left.x : right.x;
		result.y = (left.y > right.y) ? left.y : right.y;
		result.z = (left.z > right.z) ? left.z : right.z;
		result.w = (left.w > right.w) ? left.w : right.w;
	}

	Vector4 Vector4::operator + ( Vector4 left, Vector4 right )
	{
		return Vector4( left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w );
	}
	
	Vector4 Vector4::operator - ( Vector4 left, Vector4 right )
	{
		return Vector4( left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w );
	}
	
	Vector4 Vector4::operator - ( Vector4 value )
	{
		return Vector4( -value.x, -value.y, -value.z, -value.w );
	}
	
	Vector4 Vector4::operator * ( Vector4 value, float scale )
	{
		return Vector4( value.x * scale, value.y * scale, value.z * scale, value.w * scale );
	}
	
	Vector4 Vector4::operator * ( float scale, Vector4 vec )
	{
		return vec * scale;
	}
	
	Vector4 Vector4::operator / ( Vector4 value, float scale )
	{
		return Vector4( value.x / scale, value.y / scale, value.z / scale, value.w / scale );
	}

	bool Vector4::operator == ( Vector4 left, Vector4 right )
	{
		return Vector4::Equals( left, right );
	}

	bool Vector4::operator != ( Vector4 left, Vector4 right )
	{
		return !Vector4::Equals( left, right );
	}

	String^ Vector4::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "X:{0} Y:{1} Z:{2} W:{3}", x.ToString(CultureInfo::CurrentCulture), 
			y.ToString(CultureInfo::CurrentCulture), z.ToString(CultureInfo::CurrentCulture),
			w.ToString(CultureInfo::CurrentCulture) );
	}

	int Vector4::GetHashCode()
	{
		return x.GetHashCode() + y.GetHashCode() + z.GetHashCode() + w.GetHashCode();
	}

	bool Vector4::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Vector4>( value ) );
	}

	bool Vector4::Equals( Vector4 value )
	{
		return ( x == value.x && y == value.y && z == value.z && w == value.w );
	}

	bool Vector4::Equals( Vector4% value1, Vector4% value2 )
	{
		return ( value1.x == value2.x && value1.y == value2.y && value1.z == value2.z && value1.w == value2.w );
	}
}