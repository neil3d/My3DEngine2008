#include "stdafx.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vector3.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Quaternion::Quaternion( Vector3 value, float w )
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = w;
	}

	Quaternion Quaternion::Identity::get()
	{
		Quaternion result;
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
		result.w = 1.0f;
		return result;
	}

	bool Quaternion::IsIdentity::get()
	{
		if( x != 0.0f || y != 0.0f || z != 0.0f )
			return false;

		return (w == 1.0f);
	}

	//Vector3 Quaternion::Axis::get()
	//{
	//	pin_ptr<Quaternion> pinThis = this;
	//	float angle;
	//	Vector3 axis;

	//	D3DXQuaternionToAxisAngle( (D3DXQUATERNION*) pinThis, (D3DXVECTOR3*) &axis, &angle );
	//	return axis;
	//}

	//float Quaternion::Angle::get()
	//{
	//	pin_ptr<Quaternion> pinThis = this;
	//	float angle;
	//	Vector3 axis;

	//	D3DXQuaternionToAxisAngle( (D3DXQUATERNION*) pinThis, (D3DXVECTOR3*) &axis, &angle );
	//	return angle;
	//}

	float Quaternion::Length()
	{
		return static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) + (w * w) ) );
	}
	
	float Quaternion::LengthSquared()
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	void Quaternion::Normalize()
	{
		float length = 1.0f / Length();
		x *= length;
		y *= length;
		z *= length;
		w *= length;
	}

	void Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void Quaternion::Invert()
	{
		float lengthSq = 1.0f / ( (x * x) + (y * y) + (z * z) + (w * w) );
		x = -x * lengthSq;
		y = -y * lengthSq;
		z = -z * lengthSq;
		w = w * lengthSq;
	}

	Quaternion Quaternion::Add( Quaternion left, Quaternion right )
	{
		Quaternion result;
		result.x = left.x + right.x;
		result.y = left.y + right.y;
		result.z = left.z + right.z;
		result.w = left.w + right.w;
		return result;
	}

	void Quaternion::Add( Quaternion% left, Quaternion% right, [Out] Quaternion% result )
	{
		Quaternion r;
		r.x = left.x + right.x;
		r.y = left.y + right.y;
		r.z = left.z + right.z;
		r.w = left.w + right.w;

		result = r;
	}

	//Quaternion Quaternion::Barycentric( Quaternion q1, Quaternion q2, Quaternion q3, float f, float g )
	//{
	//	Quaternion result;

	//	D3DXQuaternionBaryCentric( (D3DXQUATERNION*) &result, (D3DXQUATERNION*) &q1,
	//		(D3DXQUATERNION*) &q2, (D3DXQUATERNION*) &q3, f, g );

	//	return result;
	//}

	//void Quaternion::Barycentric( Quaternion% q1, Quaternion% q2, Quaternion% q3, float f, float g, [Out] Quaternion% result )
	//{
	//	pin_ptr<Quaternion> pin1 = &q1;
	//	pin_ptr<Quaternion> pin2 = &q2;
	//	pin_ptr<Quaternion> pin3 = &q3;
	//	pin_ptr<Quaternion> pinResult = &result;

	//	D3DXQuaternionBaryCentric( (D3DXQUATERNION*) pinResult, (D3DXQUATERNION*) pin1,
	//		(D3DXQUATERNION*) pin2, (D3DXQUATERNION*) pin3, f, g );
	//}

	Quaternion Quaternion::Concatenate( Quaternion left, Quaternion right )
	{
		Quaternion quaternion;
		float rx = right.x;
		float ry = right.y;
		float rz = right.z;
		float rw = right.w;
		float lx = left.x;
		float ly = left.y;
		float lz = left.z;
		float lw = left.w;
		float yz = (ry * lz) - (rz * ly);
		float xz = (rz * lx) - (rx * lz);
		float xy = (rx * ly) - (ry * lx);
		float lengthSq = ((rx * lx) + (ry * ly)) + (rz * lz);

		quaternion.x = ((rx * lw) + (lx * rw)) + yz;
		quaternion.y = ((ry * lw) + (ly * rw)) + xz;
		quaternion.z = ((rz * lw) + (lz * rw)) + xy;
		quaternion.w = (rw * lw) - lengthSq;

		return quaternion;
	}

	void Quaternion::Concatenate( Quaternion% left, Quaternion% right, [Out] Quaternion% result )
	{
		float rx = right.x;
		float ry = right.y;
		float rz = right.z;
		float rw = right.w;
		float lx = left.x;
		float ly = left.y;
		float lz = left.z;
		float lw = left.w;
		float yz = (ry * lz) - (rz * ly);
		float xz = (rz * lx) - (rx * lz);
		float xy = (rx * ly) - (ry * lx);
		float lengthSq = ((rx * lx) + (ry * ly)) + (rz * lz);

		Quaternion r;
		r.x = ((rx * lw) + (lx * rw)) + yz;
		r.y = ((ry * lw) + (ly * rw)) + xz;
		r.z = ((rz * lw) + (lz * rw)) + xy;
		r.w = (rw * lw) - lengthSq;

		result = r;
	}

	Quaternion Quaternion::Conjugate( Quaternion quat )
	{
		Quaternion result;
		result.x = -quat.x;
		result.y = -quat.y;
		result.z = -quat.z;
		result.w = quat.w;
		return result;
	}

	void Quaternion::Conjugate( Quaternion% quat, [Out] Quaternion% result )
	{
		result.x = -quat.x;
		result.y = -quat.y;
		result.z = -quat.z;
		result.w = quat.w;
	}

	Quaternion Quaternion::Divide( Quaternion left, Quaternion right )
	{
		Quaternion result;
		result.x = left.x / right.x;
		result.y = left.y / right.y;
		result.z = left.z / right.z;
		result.w = left.w / right.w;
		return result;
	}

	void Quaternion::Divide( Quaternion% left, Quaternion% right, [Out] Quaternion% result )
	{
		result.x = left.x / right.x;
		result.y = left.y / right.y;
		result.z = left.z / right.z;
		result.w = left.w / right.w;
	}
	
	float Quaternion::Dot( Quaternion left, Quaternion right )
	{
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w); 
	}

	//Quaternion Quaternion::Exponential( Quaternion quat )
	//{
	//	Quaternion result;
	//	D3DXQuaternionExp( (D3DXQUATERNION*) &result, (D3DXQUATERNION*) &quat );
	//	return result;
	//}

	//void Quaternion::Exponential( Quaternion% quat, [Out] Quaternion% result )
	//{
	//	pin_ptr<Quaternion> pinQuat = &quat;
	//	pin_ptr<Quaternion> pinResult = &result;

	//	D3DXQuaternionExp( (D3DXQUATERNION*) pinResult, (D3DXQUATERNION*) pinQuat );
	//}

	Quaternion Quaternion::Invert( Quaternion quaternion )
	{
		Quaternion result;
		float lengthSq = 1.0f / ( (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z) + (quaternion.w * quaternion.w) );

		result.x = -quaternion.x * lengthSq;
		result.y = -quaternion.y * lengthSq;
		result.z = -quaternion.z * lengthSq;
		result.w = quaternion.w * lengthSq;

		return result;
	}

	void Quaternion::Invert( Quaternion% quaternion, [Out] Quaternion% result )
	{
		float lengthSq = 1.0f / ( (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z) + (quaternion.w * quaternion.w) );

		result.x = -quaternion.x * lengthSq;
		result.y = -quaternion.y * lengthSq;
		result.z = -quaternion.z * lengthSq;
		result.w = quaternion.w * lengthSq;
	}

	Quaternion Quaternion::Lerp( Quaternion left, Quaternion right, float amount )
	{
		Quaternion result;
		float inverse = 1.0f - amount;
		float dot = (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w); 

		if( dot >= 0.0f )
		{
			result.x = (inverse * left.x) + (amount * right.x);
			result.y = (inverse * left.y) + (amount * right.y);
			result.z = (inverse * left.z) + (amount * right.z);
			result.w = (inverse * left.w) + (amount * right.w);
		}
		else
		{
			result.x = (inverse * left.x) - (amount * right.x);
			result.y = (inverse * left.y) - (amount * right.y);
			result.z = (inverse * left.z) - (amount * right.z);
			result.w = (inverse * left.w) - (amount * right.w);
		}

		float invLength = 1.0f / result.Length();

		result.x *= invLength;
		result.y *= invLength;
		result.z *= invLength;
		result.w *= invLength;

		return result;
	}

	void Quaternion::Lerp( Quaternion% left, Quaternion% right, float amount, [Out] Quaternion% result )
	{
		float inverse = 1.0f - amount;
		float dot = (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w); 

		if( dot >= 0.0f )
		{
			result.x = (inverse * left.x) + (amount * right.x);
			result.y = (inverse * left.y) + (amount * right.y);
			result.z = (inverse * left.z) + (amount * right.z);
			result.w = (inverse * left.w) + (amount * right.w);
		}
		else
		{
			result.x = (inverse * left.x) - (amount * right.x);
			result.y = (inverse * left.y) - (amount * right.y);
			result.z = (inverse * left.z) - (amount * right.z);
			result.w = (inverse * left.w) - (amount * right.w);
		}

		float invLength = 1.0f / result.Length();

		result.x *= invLength;
		result.y *= invLength;
		result.z *= invLength;
		result.w *= invLength;
	}

	//Quaternion Quaternion::Logarithm( Quaternion quat )
	//{
	//	Quaternion result;
	//	D3DXQuaternionLn( (D3DXQUATERNION*) &result, (D3DXQUATERNION*) &quat );
	//	return result;
	//}

	//void Quaternion::Logarithm( Quaternion% quat, [Out] Quaternion% result )
	//{
	//	pin_ptr<Quaternion> pinQuat = &quat;
	//	pin_ptr<Quaternion> pinResult = &result;

	//	D3DXQuaternionLn( (D3DXQUATERNION*) pinResult, (D3DXQUATERNION*) pinQuat );
	//}

	Quaternion Quaternion::Multiply( Quaternion left, Quaternion right )
	{
		Quaternion quaternion;
		float rx = right.x;
		float ry = right.y;
		float rz = right.z;
		float rw = right.w;
		float lx = left.x;
		float ly = left.y;
		float lz = left.z;
		float lw = left.w;
		float yz = (ry * lz) - (rz * ly);
		float xz = (rz * lx) - (rx * lz);
		float xy = (rx * ly) - (ry * lx);
		float lengthSq = ((rx * lx) + (ry * ly)) + (rz * lz);

		quaternion.x = ((rx * lw) + (lx * rw)) + yz;
		quaternion.y = ((ry * lw) + (ly * rw)) + xz;
		quaternion.z = ((rz * lw) + (lz * rw)) + xy;
		quaternion.w = (rw * lw) - lengthSq;

		return quaternion;
	}

	void Quaternion::Multiply( Quaternion% left, Quaternion% right, [Out] Quaternion% result )
	{
		float rx = right.x;
		float ry = right.y;
		float rz = right.z;
		float rw = right.w;
		float lx = left.x;
		float ly = left.y;
		float lz = left.z;
		float lw = left.w;
		float yz = (ry * lz) - (rz * ly);
		float xz = (rz * lx) - (rx * lz);
		float xy = (rx * ly) - (ry * lx);
		float lengthSq = ((rx * lx) + (ry * ly)) + (rz * lz);

		result.x = ((rx * lw) + (lx * rw)) + yz;
		result.y = ((ry * lw) + (ly * rw)) + xz;
		result.z = ((rz * lw) + (lz * rw)) + xy;
		result.w = (rw * lw) - lengthSq;
	}

	Quaternion Quaternion::Multiply( Quaternion quaternion, float scale )
	{
		Quaternion result;
		result.x = quaternion.x * scale;
		result.y = quaternion.y * scale;
		result.z = quaternion.z * scale;
		result.w = quaternion.w * scale;
		return result;
	}

	void Quaternion::Multiply( Quaternion% quaternion, float scale, [Out] Quaternion% result )
	{
		result.x = quaternion.x * scale;
		result.y = quaternion.y * scale;
		result.z = quaternion.z * scale;
		result.w = quaternion.w * scale;
	}

	Quaternion Quaternion::Negate( Quaternion quat )
	{
		Quaternion result;
		result.x = -quat.x;
		result.y = -quat.y;
		result.z = -quat.z;
		result.w = -quat.w;
		return result;
	}

	void Quaternion::Negate( Quaternion% quat, [Out] Quaternion% result )
	{
		result.x = -quat.x;
		result.y = -quat.y;
		result.z = -quat.z;
		result.w = -quat.w;
	}

	Quaternion Quaternion::Normalize( Quaternion quat )
	{
		quat.Normalize();
		return quat;
	}

	void Quaternion::Normalize( Quaternion% quat, [Out] Quaternion% result )
	{
		float length = 1.0f / quat.Length();
		result.x = quat.x * length;
		result.y = quat.y * length;
		result.z = quat.z * length;
		result.w = quat.w * length;
	}

	Quaternion Quaternion::RotationAxis( Vector3 axis, float angle )
	{
		Quaternion result;

		Vector3::Normalize( axis, axis );

		float half = angle * 0.5f;
		float sin = static_cast<float>( Math::Sin( static_cast<double>( half ) ) );
		float cos = static_cast<float>( Math::Cos( static_cast<double>( half ) ) );

		result.x = axis.x * sin;
		result.y = axis.y * sin;
		result.z = axis.z * sin;
		result.w = cos;

		return result;
	}

	void Quaternion::RotationAxis( Vector3% axis, float angle, [Out] Quaternion% result )
	{
		Vector3::Normalize( axis, axis );

		float half = angle * 0.5f;
		float sin = static_cast<float>( Math::Sin( static_cast<double>( half ) ) );
		float cos = static_cast<float>( Math::Cos( static_cast<double>( half ) ) );

		result.x = axis.x * sin;
		result.y = axis.y * sin;
		result.z = axis.z * sin;
		result.w = cos;
	}
	
	Quaternion Quaternion::RotationMatrix( Matrix matrix )
	{
		Quaternion result;
		float scale = matrix.M11 + matrix.M22 + matrix.M33;

		if( scale > 0.0f )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(scale + 1.0f) ) );

			result.w = sqrt * 0.5f;
			sqrt = 0.5f / sqrt;

			result.x = (matrix.M23 - matrix.M32) * sqrt;
			result.y = (matrix.M31 - matrix.M13) * sqrt;
			result.z = (matrix.M12 - matrix.M21) * sqrt;

			return result;
		}

		if( (matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33) )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M11 - matrix.M22 - matrix.M33) ) );
			float half = 0.5f / sqrt;

			result.x = 0.5f * sqrt;
			result.y = (matrix.M12 + matrix.M21) * half;
			result.z = (matrix.M13 + matrix.M31) * half;
			result.w = (matrix.M23 - matrix.M32) * half;

			return result;
		}

		if( matrix.M22 > matrix.M33 )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M22 - matrix.M11 - matrix.M33) ) );
			float half = 0.5f / sqrt;

			result.x = (matrix.M21 + matrix.M12) * half;
			result.y = 0.5f * sqrt;
			result.z = (matrix.M32 + matrix.M23) * half;
			result.w = (matrix.M31 - matrix.M13) * half;

			return result;
		}

		float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M33 - matrix.M11 - matrix.M22) ) );
		float half = 0.5f / sqrt;

		result.x = (matrix.M31 + matrix.M13) * half;
		result.y = (matrix.M32 + matrix.M23) * half;
		result.z = 0.5f * sqrt;
		result.w = (matrix.M12 - matrix.M21) * half;

		return result;
	}

	void Quaternion::RotationMatrix( Matrix% matrix, [Out] Quaternion% result )
	{
		float scale = matrix.M11 + matrix.M22 + matrix.M33;

		if( scale > 0.0f )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(scale + 1.0f) ) );

			result.w = sqrt * 0.5f;
			sqrt = 0.5f / sqrt;

			result.x = (matrix.M23 - matrix.M32) * sqrt;
			result.y = (matrix.M31 - matrix.M13) * sqrt;
			result.z = (matrix.M12 - matrix.M21) * sqrt;
			return;
		}

		if( (matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33) )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M11 - matrix.M22 - matrix.M33) ) );
			float half = 0.5f / sqrt;

			result.x = 0.5f * sqrt;
			result.y = (matrix.M12 + matrix.M21) * half;
			result.z = (matrix.M13 + matrix.M31) * half;
			result.w = (matrix.M23 - matrix.M32) * half;
			return;
		}

		if( matrix.M22 > matrix.M33 )
		{
			float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M22 - matrix.M11 - matrix.M33) ) );
			float half = 0.5f / sqrt;

			result.x = (matrix.M21 + matrix.M12) * half;
			result.y = 0.5f * sqrt;
			result.z = (matrix.M32 + matrix.M23) * half;
			result.w = (matrix.M31 - matrix.M13) * half;
			return;
		}

		float sqrt = static_cast<float>( Math::Sqrt( static_cast<double>(1.0f + matrix.M33 - matrix.M11 - matrix.M22) ) );
		float half = 0.5f / sqrt;

		result.x = (matrix.M31 + matrix.M13) * half;
		result.y = (matrix.M32 + matrix.M23) * half;
		result.z = 0.5f * sqrt;
		result.w = (matrix.M12 - matrix.M21) * half;
	}

	Quaternion Quaternion::RotationYawPitchRoll( float yaw, float pitch, float roll )
	{
		Quaternion result;

		float halfRoll = roll * 0.5f;
		float sinRoll = static_cast<float>( Math::Sin( static_cast<double>( halfRoll ) ) );
		float cosRoll = static_cast<float>( Math::Cos( static_cast<double>( halfRoll ) ) );
		float halfPitch = pitch * 0.5f;
		float sinPitch = static_cast<float>( Math::Sin( static_cast<double>( halfPitch ) ) );
		float cosPitch = static_cast<float>( Math::Cos( static_cast<double>( halfPitch ) ) );
		float halfYaw = yaw * 0.5f;
		float sinYaw = static_cast<float>( Math::Sin( static_cast<double>( halfYaw ) ) );
		float cosYaw = static_cast<float>( Math::Cos( static_cast<double>( halfYaw ) ) );

		result.x = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
		result.y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
		result.z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
		result.w = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);

		return result;
	}

	void Quaternion::RotationYawPitchRoll( float yaw, float pitch, float roll, [Out] Quaternion% result )
	{
		float halfRoll = roll * 0.5f;
		float sinRoll = static_cast<float>( Math::Sin( static_cast<double>( halfRoll ) ) );
		float cosRoll = static_cast<float>( Math::Cos( static_cast<double>( halfRoll ) ) );
		float halfPitch = pitch * 0.5f;
		float sinPitch = static_cast<float>( Math::Sin( static_cast<double>( halfPitch ) ) );
		float cosPitch = static_cast<float>( Math::Cos( static_cast<double>( halfPitch ) ) );
		float halfYaw = yaw * 0.5f;
		float sinYaw = static_cast<float>( Math::Sin( static_cast<double>( halfYaw ) ) );
		float cosYaw = static_cast<float>( Math::Cos( static_cast<double>( halfYaw ) ) );

		result.x = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
		result.y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
		result.z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
		result.w = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);
	}

	Quaternion Quaternion::Slerp( Quaternion q1, Quaternion q2, float t )
	{
		Quaternion result;

		float opposite;
		float inverse;
		float dot = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
		bool flag = false;

		if( dot < 0.0f )
		{
			flag = true;
			dot = -dot;
		}

		if( dot > 0.999999f )
		{
			inverse = 1.0f - t;
			opposite = flag ? -t : t;
		}
		else
		{
			float acos = static_cast<float>( Math::Acos( static_cast<double>( dot ) ) );
			float invSin = static_cast<float>( ( 1.0f / Math::Sin( static_cast<double>( acos ) ) ) );

			inverse = ( static_cast<float>( Math::Sin( static_cast<double>( (1.0f - t) * acos ) ) ) ) * invSin;
			opposite = flag ? ( ( static_cast<float>( -Math::Sin( static_cast<double>( t * acos ) ) ) ) * invSin ) : ( ( static_cast<float>( Math::Sin( static_cast<double>( t * acos ) ) ) ) * invSin );
		}

		result.x = (inverse * q1.x) + (opposite * q2.x);
		result.y = (inverse * q1.y) + (opposite * q2.y);
		result.z = (inverse * q1.z) + (opposite * q2.z);
		result.w = (inverse * q1.w) + (opposite * q2.w);

		return result;
	}

	void Quaternion::Slerp( Quaternion% q1, Quaternion% q2, float t, [Out] Quaternion% result )
	{
		float opposite;
		float inverse;
		float dot = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
		bool flag = false;

		if( dot < 0.0f )
		{
			flag = true;
			dot = -dot;
		}

		if( dot > 0.999999f )
		{
			inverse = 1.0f - t;
			opposite = flag ? -t : t;
		}
		else
		{
			float acos = static_cast<float>( Math::Acos( static_cast<double>( dot ) ) );
			float invSin = static_cast<float>( ( 1.0f / Math::Sin( static_cast<double>( acos ) ) ) );

			inverse = ( static_cast<float>( Math::Sin( static_cast<double>( (1.0f - t) * acos ) ) ) ) * invSin;
			opposite = flag ? ( ( static_cast<float>( -Math::Sin( static_cast<double>( t * acos ) ) ) ) * invSin ) : ( ( static_cast<float>( Math::Sin( static_cast<double>( t * acos ) ) ) ) * invSin );
		}

		result.x = (inverse * q1.x) + (opposite * q2.x);
		result.y = (inverse * q1.y) + (opposite * q2.y);
		result.z = (inverse * q1.z) + (opposite * q2.z);
		result.w = (inverse * q1.w) + (opposite * q2.w);
	}

	//Quaternion Quaternion::Squad( Quaternion q1, Quaternion a, Quaternion b, Quaternion c, float t )
	//{
	//	Quaternion result;

	//	D3DXQuaternionSquad( (D3DXQUATERNION*) &result, (D3DXQUATERNION*) &q1, (D3DXQUATERNION*) &a,
	//		(D3DXQUATERNION*) &b, (D3DXQUATERNION*) &c, t );

	//	return result;
	//}

	//void Quaternion::Squad( Quaternion% q1, Quaternion% a, Quaternion% b, Quaternion% c, float t, [Out] Quaternion% result )
	//{
	//	pin_ptr<Quaternion> pin1 = &q1;
	//	pin_ptr<Quaternion> pinA = &a;
	//	pin_ptr<Quaternion> pinB = &b;
	//	pin_ptr<Quaternion> pinC = &c;
	//	pin_ptr<Quaternion> pinResult = &result;

	//	D3DXQuaternionSquad( (D3DXQUATERNION*) pinResult, (D3DXQUATERNION*) pin1, (D3DXQUATERNION*) pinA,
	//		(D3DXQUATERNION*) pinB, (D3DXQUATERNION*) pinC, t );
	//}

	//array<Quaternion>^ Quaternion::SquadSetup( Quaternion source1, Quaternion source2, Quaternion source3, Quaternion source4 )
	//{
	//	Quaternion result1;
	//	Quaternion result2;
	//	Quaternion result3;
	//	array<Quaternion>^ results = gcnew array<Quaternion>( 3 );

	//	D3DXQuaternionSquadSetup( (D3DXQUATERNION*) &result1, (D3DXQUATERNION*) &result2, (D3DXQUATERNION*) &result3,
	//		(D3DXQUATERNION*) &source1, (D3DXQUATERNION*) &source2, (D3DXQUATERNION*) &source3, (D3DXQUATERNION*) &source4 );

	//	results[0] = result1;
	//	results[1] = result2;
	//	results[2] = result3;
	//	return results;
	//}

	Quaternion Quaternion::Subtract( Quaternion left, Quaternion right )
	{
		Quaternion result;
		result.x = left.x - right.x;
		result.y = left.y - right.y;
		result.z = left.z - right.z;
		result.w = left.w - right.w;
		return result;
	}

	void Quaternion::Subtract( Quaternion% left, Quaternion% right, [Out] Quaternion% result )
	{
		result.x = left.x - right.x;
		result.y = left.y - right.y;
		result.z = left.z - right.z;
		result.w = left.w - right.w;
	}

	Quaternion Quaternion::operator * (Quaternion left, Quaternion right)
	{
		Quaternion quaternion;
		float rx = right.x;
		float ry = right.y;
		float rz = right.z;
		float rw = right.w;
		float lx = left.x;
		float ly = left.y;
		float lz = left.z;
		float lw = left.w;
		float yz = (ry * lz) - (rz * ly);
		float xz = (rz * lx) - (rx * lz);
		float xy = (rx * ly) - (ry * lx);
		float lengthSq = ((rx * lx) + (ry * ly)) + (rz * lz);

		quaternion.x = ((rx * lw) + (lx * rw)) + yz;
		quaternion.y = ((ry * lw) + (ly * rw)) + xz;
		quaternion.z = ((rz * lw) + (lz * rw)) + xy;
		quaternion.w = (rw * lw) - lengthSq;

		return quaternion;
	}

	Quaternion Quaternion::operator * (Quaternion quaternion, float scale)
	{
		Quaternion result;
		result.x = quaternion.x * scale;
		result.y = quaternion.y * scale;
		result.z = quaternion.z * scale;
		result.w = quaternion.w * scale;
		return result;
	}

	Quaternion Quaternion::operator * (float scale, Quaternion quaternion)
	{
		Quaternion result;
		result.x = quaternion.x * scale;
		result.y = quaternion.y * scale;
		result.z = quaternion.z * scale;
		result.w = quaternion.w * scale;
		return result;
	}

	Quaternion Quaternion::operator / (Quaternion lhs, float rhs)
	{
		Quaternion result;
		result.x = lhs.x / rhs;
		result.y = lhs.y / rhs;
		result.z = lhs.z / rhs;
		result.w = lhs.w / rhs;
		return result;
	}

	Quaternion Quaternion::operator + (Quaternion lhs, Quaternion rhs)
	{
		Quaternion result;
		result.x = lhs.x + rhs.x;
		result.y = lhs.y + rhs.y;
		result.z = lhs.z + rhs.z;
		result.w = lhs.w + rhs.w;
		return result;
	}

	Quaternion Quaternion::operator - (Quaternion lhs, Quaternion rhs)
	{
		Quaternion result;
		result.x = lhs.x * rhs.x;
		result.y = lhs.y * rhs.y;
		result.z = lhs.z * rhs.z;
		result.w = lhs.w * rhs.w;
		return result;
	}

	Quaternion Quaternion::operator - (Quaternion quaternion)
	{
		Quaternion result;
		result.x = -quaternion.x;
		result.y = -quaternion.y;
		result.z = -quaternion.z;
		result.w = -quaternion.w;
		return result;
	}

	bool Quaternion::operator == ( Quaternion left, Quaternion right )
	{
		return Quaternion::Equals( left, right );
	}

	bool Quaternion::operator != ( Quaternion left, Quaternion right )
	{
		return !Quaternion::Equals( left, right );
	}

	String^ Quaternion::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "X:{0} Y:{1} Z:{2} W:{3}", x.ToString(CultureInfo::CurrentCulture), 
			y.ToString(CultureInfo::CurrentCulture), z.ToString(CultureInfo::CurrentCulture),
			w.ToString(CultureInfo::CurrentCulture) );
	}

	int Quaternion::GetHashCode()
	{
		return x.GetHashCode() + y.GetHashCode() + z.GetHashCode() + w.GetHashCode();
	}

	bool Quaternion::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Quaternion>( value ) );
	}

	bool Quaternion::Equals( Quaternion value )
	{
		return ( x == value.x && y == value.y && z == value.z && w == value.w );
	}

	bool Quaternion::Equals( Quaternion% value1, Quaternion% value2 )
	{
		return ( value1.x == value2.x && value1.y == value2.y && value1.z == value2.z && value1.w == value2.w );
	}
}
