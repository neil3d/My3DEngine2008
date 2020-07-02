#include "stdafx.h"
//#include "../Utilities.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Vector3::Vector3( float value )
	{
		x = value;
		y = value;
		z = value;
	}
	
	Vector3::Vector3( Vector2 value, float _z )
	{
		x = value.x;
		y = value.y;
		z = _z;
	}
	
	Vector3::Vector3( float _x, float _y, float _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
	float Vector3::default::get( int index )
	{
		switch( index )
		{
		case 0:
			return x;

		case 1:
			return y;

		case 2:
			return z;

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector3 run from 0 to 2, inclusive." );
		}
	}
	
	void Vector3::default::set( int index, float value )
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

		default:
			throw gcnew ArgumentOutOfRangeException( "index", "Indices for Vector3 run from 0 to 2, inclusive." );
		}
	}
	
	float Vector3::Length()
	{
		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) ) );
	}
	
	float Vector3::LengthSquared()
	{
		return (x * x) + (y * y) + (z * z);
	}

	void Vector3::Normalize()
	{
		float length = Length();
		if( length == 0 )
			return;
		float num = 1 / length;
		x *= num;
		y *= num;
		z *= num;
	}
	
	Vector3 Vector3::Add( Vector3 left, Vector3 right )
	{
		return Vector3( left.x + right.x, left.y + right.y, left.z + right.z );
	}
	
	void Vector3::Add( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		result = Vector3( left.x + right.x, left.y + right.y, left.z + right.z );
	}
	
	Vector3 Vector3::Subtract( Vector3 left, Vector3 right )
	{
		return Vector3( left.x - right.x, left.y - right.y, left.z - right.z );
	}
	
	void Vector3::Subtract( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		result = Vector3( left.x - right.x, left.y - right.y, left.z - right.z );
	}
	
	Vector3 Vector3::Modulate( Vector3 left, Vector3 right )
	{
		return Vector3( left.x * right.x, left.y * right.y, left.z * right.z );
	}
	
	void Vector3::Modulate( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		result = Vector3( left.x * right.x, left.y * right.y, left.z * right.z );
	}
	
	Vector3 Vector3::Multiply( Vector3 value, float scale )
	{
		return Vector3( value.x * scale, value.y * scale, value.z * scale );
	}
	
	void Vector3::Multiply( Vector3% value, float scale, [Out] Vector3% result )
	{
		result = Vector3( value.x * scale, value.y * scale, value.z * scale );
	}
	
	Vector3 Vector3::Divide( Vector3 value, float scale )
	{
		return Vector3( value.x / scale, value.y / scale, value.z / scale );
	}
	
	void Vector3::Divide( Vector3% value, float scale, [Out] Vector3% result )
	{
		result = Vector3( value.x / scale, value.y / scale, value.z / scale );
	}
	
	Vector3 Vector3::Negate( Vector3 value )
	{
		return Vector3( -value.x, -value.y, -value.z );
	}
	
	void Vector3::Negate( Vector3% value, [Out] Vector3% result )
	{
		result = Vector3( -value.x, -value.y, -value.z );
	}
	
	Vector3 Vector3::Barycentric( Vector3 value1, Vector3 value2, Vector3 value3, float amount1, float amount2 )
	{
		Vector3 vector;
		vector.x = (value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x));
		vector.y = (value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y));
		vector.z = (value1.z + (amount1 * (value2.z - value1.z))) + (amount2 * (value3.z - value1.z));
		return vector;
	}
	
	void Vector3::Barycentric( Vector3% value1, Vector3% value2, Vector3% value3, float amount1, float amount2, [Out] Vector3% result )
	{
		result = Vector3((value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x)),
			(value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y)),
			(value1.z + (amount1 * (value2.z - value1.z))) + (amount2 * (value3.z - value1.z)) );
	}
	
	Vector3 Vector3::CatmullRom( Vector3 value1, Vector3 value2, Vector3 value3, Vector3 value4, float amount )
	{
		Vector3 vector;
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

		return vector;
	}
	
	void Vector3::CatmullRom( Vector3% value1, Vector3% value2, Vector3% value3, Vector3% value4, float amount, [Out] Vector3% result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		
		Vector3 r;

		r.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		r.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		r.z = 0.5f * ((((2.0f * value2.z) + ((-value1.z + value3.z) * amount)) + 
			(((((2.0f * value1.z) - (5.0f * value2.z)) + (4.0f * value3.z)) - value4.z) * squared)) + 
			((((-value1.z + (3.0f * value2.z)) - (3.0f * value3.z)) + value4.z) * cubed));

		result = r;
	}
	
	Vector3 Vector3::Clamp( Vector3 value, Vector3 min, Vector3 max )
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

		return Vector3( x, y, z );
	}
	
	void Vector3::Clamp( Vector3% value, Vector3% min, Vector3% max, [Out] Vector3% result )
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

		result = Vector3( x, y, z );
	}
	
	Vector3 Vector3::Hermite( Vector3 value1, Vector3 tangent1, Vector3 value2, Vector3 tangent2, float amount )
	{
		Vector3 vector;
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		vector.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		vector.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);
		vector.z = (((value1.z * part1) + (value2.z * part2)) + (tangent1.z * part3)) + (tangent2.z * part4);

		return vector;
	}
	
	void Vector3::Hermite( Vector3% value1, Vector3% tangent1, Vector3% value2, Vector3% tangent2, float amount, [Out] Vector3% result )
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
	}
	
	Vector3 Vector3::Lerp( Vector3 start, Vector3 end, float factor )
	{
		Vector3 vector;

		vector.x = start.x + ((end.x - start.x) * factor);
		vector.y = start.y + ((end.y - start.y) * factor);
		vector.z = start.z + ((end.z - start.z) * factor);

		return vector;
	}
	
	void Vector3::Lerp( Vector3% start, Vector3% end, float factor, [Out] Vector3% result )
	{
		result.x = start.x + ((end.x - start.x) * factor);
		result.y = start.y + ((end.y - start.y) * factor);
		result.z = start.z + ((end.z - start.z) * factor);
	}
	
	Vector3 Vector3::SmoothStep( Vector3 start, Vector3 end, float amount )
	{
		Vector3 vector;

		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		vector.x = start.x + ((end.x - start.x) * amount);
		vector.y = start.y + ((end.y - start.y) * amount);
		vector.z = start.z + ((end.z - start.z) * amount);

		return vector;
	}
	
	void Vector3::SmoothStep( Vector3% start, Vector3% end, float amount, [Out] Vector3% result )
	{
		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		result.x = start.x + ((end.x - start.x) * amount);
		result.y = start.y + ((end.y - start.y) * amount);
		result.z = start.z + ((end.z - start.z) * amount);
	}
	
	float Vector3::Distance( Vector3 value1, Vector3 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;
		float z = value1.z - value2.z;

		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) ) );
	}
	
	float Vector3::DistanceSquared( Vector3 value1, Vector3 value2 )
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;
		float z = value1.z - value2.z;

		return (x * x) + (y * y) + (z * z);
	}
	
	float Vector3::Dot( Vector3 left, Vector3 right )
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z);
	}
	
	Vector3 Vector3::Cross( Vector3 left, Vector3 right )
	{
		Vector3 result;
		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;
		return result;
	}
	
	void Vector3::Cross( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		Vector3 r;
		r.x = left.y * right.z - left.z * right.y;
		r.y = left.z * right.x - left.x * right.z;
		r.z = left.x * right.y - left.y * right.x; 

		result = r;
	}
	
	Vector3 Vector3::Reflect( Vector3 vector, Vector3 normal )
	{
		Vector3 result;
		float dot = ((vector.x * normal.x) + (vector.y * normal.y)) + (vector.z * normal.z);

		result.x = vector.x - ((2.0f * dot) * normal.x);
		result.y = vector.y - ((2.0f * dot) * normal.y);
		result.z = vector.z - ((2.0f * dot) * normal.z);

		return result;
	}
	
	void Vector3::Reflect( Vector3% vector, Vector3% normal, [Out] Vector3% result )
	{
		float dot = ((vector.x * normal.x) + (vector.y * normal.y)) + (vector.z * normal.z);

		result.x = vector.x - ((2.0f * dot) * normal.x);
		result.y = vector.y - ((2.0f * dot) * normal.y);
		result.z = vector.z - ((2.0f * dot) * normal.z);
	}
	
	Vector3 Vector3::Normalize( Vector3 vector )
	{
		vector.Normalize();
		return vector;
	}
	
	void Vector3::Normalize( Vector3% vector, [Out] Vector3% result )
	{
		result = Vector3(vector);
		result.Normalize();
	}
	
	Vector4 Vector3::Transform( Vector3 vector, Matrix transform )
	{
		Vector4 result;

		result.x = (((vector.x * transform.M11) + (vector.y * transform.M21)) + (vector.z * transform.M31)) + transform.M41;
		result.y = (((vector.x * transform.M12) + (vector.y * transform.M22)) + (vector.z * transform.M32)) + transform.M42;
		result.z = (((vector.x * transform.M13) + (vector.y * transform.M23)) + (vector.z * transform.M33)) + transform.M43;
		result.w = (((vector.x * transform.M14) + (vector.y * transform.M24)) + (vector.z * transform.M34)) + transform.M44;

		return result;
	}
	
	void Vector3::Transform( Vector3% vector, Matrix% transform, [Out] Vector4% result )
	{
		result = Vector4();
		result.x = (((vector.x * transform.M11) + (vector.y * transform.M21)) + (vector.z * transform.M31)) + transform.M41;
		result.y = (((vector.x * transform.M12) + (vector.y * transform.M22)) + (vector.z * transform.M32)) + transform.M42;
		result.z = (((vector.x * transform.M13) + (vector.y * transform.M23)) + (vector.z * transform.M33)) + transform.M43;
		result.w = (((vector.x * transform.M14) + (vector.y * transform.M24)) + (vector.z * transform.M34)) + transform.M44;
	}
	
	/*void Vector3::Transform( Vector3* vectorsIn, int inputStride, Matrix* transformation, Vector4* vectorsOut, int outputStride, int count )
	{
		D3DXVec3TransformArray( reinterpret_cast<D3DXVECTOR4*>( vectorsOut ), outputStride,
			reinterpret_cast<const D3DXVECTOR3*>( vectorsIn ), inputStride,
			reinterpret_cast<const D3DXMATRIX*>( transformation ), count );
	}*/

	//void Vector3::Transform( array<Vector3>^ vectorsIn, Matrix% transformation, array<Vector4>^ vectorsOut, int offset, int count )
	//{
	//	if(vectorsIn->Length != vectorsOut->Length)
	//		throw gcnew ArgumentException( "Input and output arrays must be the same size.", "vectorsOut" );
	//	Utilities::CheckArrayBounds( vectorsIn, offset, count );

	//	pin_ptr<Vector3> pinnedIn = &vectorsIn[offset];
	//	pin_ptr<Matrix> pinnedMatrix = &transformation;
	//	pin_ptr<Vector4> pinnedOut = &vectorsOut[offset];

	//	Transform( pinnedIn, pinnedMatrix, pinnedOut, count );
	//}

	//array<Vector4>^ Vector3::Transform( array<Vector3>^ vectors, Matrix% transform )
	//{
	//	int count = vectors->Length;
	//	array<Vector4>^ results = gcnew array<Vector4>( count );

	//	/*for( int i = 0; i < count; i++ )
	//	{
	//		Vector4 r;
	//		r.x = (((vectors[i].x * transform.M11) + (vectors[i].y * transform.M21)) + (vectors[i].z * transform.M31)) + transform.M41;
	//		r.y = (((vectors[i].x * transform.M12) + (vectors[i].y * transform.M22)) + (vectors[i].z * transform.M32)) + transform.M42;
	//		r.z = (((vectors[i].x * transform.M13) + (vectors[i].y * transform.M23)) + (vectors[i].z * transform.M33)) + transform.M43;
	//		r.w = (((vectors[i].x * transform.M14) + (vectors[i].y * transform.M24)) + (vectors[i].z * transform.M34)) + transform.M44;
	//	
	//		results[i] = r;
	//	}*/
	//	Transform( vectors, transform, results );
	//	return results;
	//}
	
	Vector4 Vector3::Transform( Vector3 value, Quaternion rotation )
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
		vector.w = 1.0f;

		return vector;
	}
	
	void Vector3::Transform( Vector3% value, Quaternion% rotation, [Out] Vector4% result )
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

		result = Vector4();
		result.x = ((value.x * ((1.0f - yy) - zz)) + (value.y * (xy - wz))) + (value.z * (xz + wy));
		result.y = ((value.x * (xy + wz)) + (value.y * ((1.0f - xx) - zz))) + (value.z * (yz - wx));
		result.z = ((value.x * (xz - wy)) + (value.y * (yz + wx))) + (value.z * ((1.0f - xx) - yy));
		result.w = 1.0f;
	}
	
	array<Vector4>^ Vector3::Transform( array<Vector3>^ vectors, Quaternion% rotation )
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
			r.w = 1.0f;

			results[i] = r;
		}

		return results;
	}
	
	Vector3 Vector3::TransformCoordinate( Vector3 coord, Matrix transform )
	{
		Vector4 vector;

		vector.x = (((coord.x * transform.M11) + (coord.y * transform.M21)) + (coord.z * transform.M31)) + transform.M41;
		vector.y = (((coord.x * transform.M12) + (coord.y * transform.M22)) + (coord.z * transform.M32)) + transform.M42;
		vector.z = (((coord.x * transform.M13) + (coord.y * transform.M23)) + (coord.z * transform.M33)) + transform.M43;
		vector.w = 1 / ((((coord.x * transform.M14) + (coord.y * transform.M24)) + (coord.z * transform.M34)) + transform.M44);

		return Vector3( vector.x * vector.w, vector.y * vector.w, vector.z * vector.w );
	}
	
	void Vector3::TransformCoordinate( Vector3% coord, Matrix% transform, [Out] Vector3% result )
	{
		Vector4 vector;

		vector.x = (((coord.x * transform.M11) + (coord.y * transform.M21)) + (coord.z * transform.M31)) + transform.M41;
		vector.y = (((coord.x * transform.M12) + (coord.y * transform.M22)) + (coord.z * transform.M32)) + transform.M42;
		vector.z = (((coord.x * transform.M13) + (coord.y * transform.M23)) + (coord.z * transform.M33)) + transform.M43;
		vector.w = 1 / ((((coord.x * transform.M14) + (coord.y * transform.M24)) + (coord.z * transform.M34)) + transform.M44);

		result = Vector3( vector.x * vector.w, vector.y * vector.w, vector.z * vector.w );
	}
	
	/*void Vector3::TransformCoordinate( Vector3* coordsIn, int inputStride, Matrix* transformation, Vector3* coordsOut, int outputStride, int count )
	{
		D3DXVec3TransformCoordArray( reinterpret_cast<D3DXVECTOR3*>( coordsOut ), outputStride,
			reinterpret_cast<const D3DXVECTOR3*>( coordsIn ), inputStride,
			reinterpret_cast<const D3DXMATRIX*>( transformation ), count );
	}*/

	//void Vector3::TransformCoordinate( array<Vector3>^ coordsIn, Matrix% transformation, array<Vector3>^ coordsOut, int offset, int count )
	//{
	//	if(coordsIn->Length != coordsOut->Length)
	//		throw gcnew ArgumentException( "Input and output arrays must be the same size.", "coordinatesOut" );
	//	Utilities::CheckArrayBounds( coordsIn, offset, count );

	//	pin_ptr<Vector3> pinnedIn = &coordsIn[offset];
	//	pin_ptr<Matrix> pinnedMatrix = &transformation;
	//	pin_ptr<Vector3> pinnedOut = &coordsOut[offset];

	//	TransformCoordinate( pinnedIn, pinnedMatrix, pinnedOut, count );
	//}
	
	//array<Vector3>^ Vector3::TransformCoordinate( array<Vector3>^ coords, Matrix% transform )
	//{
	//	if( coords == nullptr )
	//		throw gcnew ArgumentNullException( "coordinates" );

	//	Vector4 vector;
	//	int count = coords->Length;
	//	array<Vector3>^ results = gcnew array<Vector3>( count );

	//	/*for( int i = 0; i < count; i++ )
	//	{
	//		vector.x = (((coords[i].x * transform.M11) + (coords[i].y * transform.M21)) + (coords[i].z * transform.M31)) + transform.M41;
	//		vector.y = (((coords[i].x * transform.M12) + (coords[i].y * transform.M22)) + (coords[i].z * transform.M32)) + transform.M42;
	//		vector.z = (((coords[i].x * transform.M13) + (coords[i].y * transform.M23)) + (coords[i].z * transform.M33)) + transform.M43;
	//		vector.w = 1 / ((((coords[i].x * transform.M14) + (coords[i].y * transform.M24)) + (coords[i].z * transform.M34)) + transform.M44);
	//		results[i] = Vector3( vector.x * vector.w, vector.y * vector.w, vector.z * vector.w );
	//	}*/
	//	TransformCoordinate( coords, transform, results );
	//	return results;
	//}
	
	Vector3 Vector3::TransformNormal( Vector3 normal, Matrix transform )
	{
		Vector3 vector;

		vector.x = ((normal.x * transform.M11) + (normal.y * transform.M21)) + (normal.z * transform.M31);
		vector.y = ((normal.x * transform.M12) + (normal.y * transform.M22)) + (normal.z * transform.M32);
		vector.z = ((normal.x * transform.M13) + (normal.y * transform.M23)) + (normal.z * transform.M33);

		return vector;
	}
	
	void Vector3::TransformNormal( Vector3% normal, Matrix% transform, [Out] Vector3% result )
	{
		result.x = ((normal.x * transform.M11) + (normal.y * transform.M21)) + (normal.z * transform.M31);
		result.y = ((normal.x * transform.M12) + (normal.y * transform.M22)) + (normal.z * transform.M32);
		result.z = ((normal.x * transform.M13) + (normal.y * transform.M23)) + (normal.z * transform.M33);
	}
	
	//void Vector3::TransformNormal( Vector3* normalsIn, int inputStride, Matrix* transformation, Vector3* normalsOut, int outputStride, int count )
	//{
	//	D3DXVec3TransformNormalArray( reinterpret_cast<D3DXVECTOR3*>( normalsOut ), outputStride,
	//		reinterpret_cast<const D3DXVECTOR3*>( normalsIn ), inputStride,
	//		reinterpret_cast<const D3DXMATRIX*>( transformation ), count );
	//}

	/*void Vector3::TransformNormal( array<Vector3>^ normalsIn, Matrix% transformation, array<Vector3>^ normalsOut, int offset, int count )
	{
		if(normalsIn->Length != normalsOut->Length)
			throw gcnew ArgumentException( "Input and output arrays must be the same size.", "normalsOut" );
		Utilities::CheckArrayBounds( normalsOut, offset, count );

		pin_ptr<Vector3> pinnedIn = &normalsIn[offset];
		pin_ptr<Matrix> pinnedMatrix = &transformation;
		pin_ptr<Vector3> pinnedOut = &normalsOut[offset];

		TransformNormal( pinnedIn, pinnedMatrix, pinnedOut, count );
	}*/
	//
	//array<Vector3>^ Vector3::TransformNormal( array<Vector3>^ normals, Matrix% transform )
	//{
	//	if( normals == nullptr )
	//		throw gcnew ArgumentNullException( "normals" );

	//	int count = normals->Length;
	//	array<Vector3>^ results = gcnew array<Vector3>( count );

	//	/*for( int i = 0; i < count; i++ )
	//	{
	//		Vector3 r;
	//		r.x = ((normals[i].x * transform.M11) + (normals[i].y * transform.M21)) + (normals[i].z * transform.M31);
	//		r.y = ((normals[i].x * transform.M12) + (normals[i].y * transform.M22)) + (normals[i].z * transform.M32);
	//		r.z = ((normals[i].x * transform.M13) + (normals[i].y * transform.M23)) + (normals[i].z * transform.M33);
	//	
	//		results[i] = r;
	//	}*/
	//	TransformNormal( normals, transform, results );
	//	return results;
	//}
	
	Vector3 Vector3::Project( Vector3 vector, int x, int y, int width, int height, float minZ, float maxZ, Matrix projection, Matrix view, Matrix world )
	{
		Matrix matrix;
		Matrix::Multiply( world, view, matrix );
		Matrix::Multiply( matrix, projection, matrix );

		float w = ( vector.x * matrix.M14 ) + ( vector.y * matrix.M24 ) + ( vector.z * matrix.M34 ) + matrix.M44;
		if( w - 1.0f < -float::Epsilon || w - 1.0f > float::Epsilon )
			vector /= w;

		return Vector3( ( ( vector.x + 1.0f ) * 0.5f * width ) + x, ( ( -vector.y + 1.0f ) * 0.5f * height ) + y, ( vector.z * ( maxZ - minZ ) ) + minZ );
	}
	
	void Vector3::Project( Vector3% vector, int x, int y, int width, int height, float minZ, float maxZ, Matrix% projection, Matrix% view, Matrix% world, [Out] Vector3% result )
	{
		Vector3 v = vector;
		Matrix matrix;
		Matrix::Multiply( world, view, matrix );
		Matrix::Multiply( matrix, projection, matrix );

		float w = ( vector.x * matrix.M14 ) + ( vector.y * matrix.M24 ) + ( vector.z * matrix.M34 ) + matrix.M44;
		if( w - 1.0f < -float::Epsilon || w - 1.0f > float::Epsilon )
			v /= w;

		result = Vector3( ( ( v.x + 1.0f ) * 0.5f * width ) + x, ( ( -v.y + 1.0f ) * 0.5f * height ) + y, ( v.z * ( maxZ - minZ ) ) + minZ );
	}
	
	Vector3 Vector3::Unproject( Vector3 vector, int x, int y, int width, int height, float minZ, float maxZ, Matrix projection, Matrix view, Matrix world )
	{
		Vector3 v;
		Matrix matrix;
		Matrix::Multiply( world, view, matrix );
		Matrix::Multiply( matrix, projection, matrix );
		Matrix::Invert( matrix, matrix );

		v.x = ( ( ( vector.x - x ) / static_cast<float>( width ) ) * 2.0f ) - 1.0f;
		v.y = -( ( ( ( vector.y - y ) / static_cast<float>( height ) ) * 2.0f ) - 1.0f );
		v.z = ( vector.z - minZ ) / ( maxZ - minZ );

		Vector4 r;
		Vector3::Transform( v, matrix, r );
		v.x = r.x;
		v.y = r.y;
		v.z = r.z;

		float w = ( vector.x * matrix.M14 ) + ( vector.y * matrix.M24 ) + ( vector.z * matrix.M34 ) + matrix.M44;
		if( w - 1.0f < -float::Epsilon || w - 1.0f > float::Epsilon )
			v /= w;

		return v;
	}
	
	void Vector3::Unproject( Vector3% vector, int x, int y, int width, int height, float minZ, float maxZ, Matrix% projection, Matrix% view, Matrix% world, [Out] Vector3% result )
	{
		Vector3 v;
		Matrix matrix;
		Matrix::Multiply( world, view, matrix );
		Matrix::Multiply( matrix, projection, matrix );
		Matrix::Invert( matrix, matrix );

		v.x = ( ( ( vector.x - x ) / static_cast<float>( width ) ) * 2.0f ) - 1.0f;
		v.y = -( ( ( ( vector.y - y ) / static_cast<float>( height ) ) * 2.0f ) - 1.0f );
		v.z = ( vector.z - minZ ) / ( maxZ - minZ );

		Vector4 r;
		Vector3::Transform( v, matrix, r );
		v.x = r.x;
		v.y = r.y;
		v.z = r.z;

		float w = ( vector.x * matrix.M14 ) + ( vector.y * matrix.M24 ) + ( vector.z * matrix.M34 ) + matrix.M44;
		if( w - 1.0f < -float::Epsilon || w - 1.0f > float::Epsilon )
			v /= w;

		result = v;
	}
	
	Vector3 Vector3::Minimize( Vector3 left, Vector3 right )
	{
		Vector3 vector;
		vector.x = (left.x < right.x) ? left.x : right.x;
		vector.y = (left.y < right.y) ? left.y : right.y;
		vector.z = (left.z < right.z) ? left.z : right.z;
		return vector;
	}
	
	void Vector3::Minimize( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		result.x = (left.x < right.x) ? left.x : right.x;
		result.y = (left.y < right.y) ? left.y : right.y;
		result.z = (left.z < right.z) ? left.z : right.z;
	}
	
	Vector3 Vector3::Maximize( Vector3 left, Vector3 right )
	{
		Vector3 vector;
		vector.x = (left.x > right.x) ? left.x : right.x;
		vector.y = (left.y > right.y) ? left.y : right.y;
		vector.z = (left.z > right.z) ? left.z : right.z;
		return vector;
	}
	
	void Vector3::Maximize( Vector3% left, Vector3% right, [Out] Vector3% result )
	{
		result.x = (left.x > right.x) ? left.x : right.x;
		result.y = (left.y > right.y) ? left.y : right.y;
		result.z = (left.z > right.z) ? left.z : right.z;
	}
	
	Vector3 Vector3::operator + ( Vector3 left, Vector3 right )
	{
		return Vector3( left.x + right.x, left.y + right.y, left.z + right.z );
	}
	
	Vector3 Vector3::operator - ( Vector3 left, Vector3 right )
	{
		return Vector3( left.x - right.x, left.y - right.y, left.z - right.z );
	}
	
	Vector3 Vector3::operator - ( Vector3 value )
	{
		return Vector3( -value.x, -value.y, -value.z );
	}

	Vector3 Vector3::operator * ( Vector3 value, float scale )
	{
		return Vector3( value.x * scale, value.y * scale, value.z * scale );
	}

	Vector3 Vector3::operator * ( float scale, Vector3 vec )
	{
		return vec * scale;
	}
	
	Vector3 Vector3::operator / ( Vector3 value, float scale )
	{
		return Vector3( value.x / scale, value.y / scale, value.z / scale );
	}
	
	bool Vector3::operator == ( Vector3 left, Vector3 right )
	{
		return Vector3::Equals( left, right );
	}
	
	bool Vector3::operator != ( Vector3 left, Vector3 right )
	{
		return !Vector3::Equals( left, right );
	}

	String^ Vector3::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "X:{0} Y:{1} Z:{2}", x.ToString(CultureInfo::CurrentCulture), y.ToString(CultureInfo::CurrentCulture), z.ToString(CultureInfo::CurrentCulture) );
	}

	int Vector3::GetHashCode()
	{
		return x.GetHashCode() + y.GetHashCode() + z.GetHashCode();
	}

	bool Vector3::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Vector3>( value ) );
	}

	bool Vector3::Equals( Vector3 value )
	{
		return ( x == value.x && y == value.y && z == value.z );
	}

	bool Vector3::Equals( Vector3% value1, Vector3% value2 )
	{
		return ( value1.x == value2.x && value1.y == value2.y && value1.z == value2.z );
	}
}