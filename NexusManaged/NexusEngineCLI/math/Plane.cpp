#include "stdafx.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Matrix.h"
#include "Plane.h"
#include "Quaternion.h"
#include "Vector3.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Plane::Plane( float a, float b, float c, float d )
	{
		Normal = Vector3( a, b, c );
		D = d;
	}
	
	Plane::Plane( Vector3 normal, float d )
	{
		Normal = normal;
		D = d;
	}

	Plane::Plane( Vector3 point, Vector3 normal )
	{
		Normal = normal;
		D = -Vector3::Dot( normal, point );
	}
	
	Plane::Plane( Vector3 point1, Vector3 point2, Vector3 point3 )
	{
		float x1 = point2.x - point1.x;
		float y1 = point2.y - point1.y;
		float z1 = point2.z - point1.z;
		float x2 = point3.x - point1.x;
		float y2 = point3.y - point1.y;
		float z2 = point3.z - point1.z;
		float yz = (y1 * z2) - (z1 * y2);
		float xz = (z1 * x2) - (x1 * z2);
		float xy = (x1 * y2) - (y1 * x2);
		float invPyth = 1.0f / static_cast<float>( Math::Sqrt((yz * yz) + (xz * xz) + (xy * xy)) );

		Normal.x = yz * invPyth;
		Normal.y = xz * invPyth;
		Normal.z = xy * invPyth;
		D = -((Normal.x * point1.x) + (Normal.y * point1.y) + (Normal.z * point1.z));
	}
	
	Plane::Plane( Vector4 value )
	{
		Normal = Vector3( value.x, value.y, value.z );
		D = value.w;
	}
	
	float Plane::Dot( Plane plane, Vector4 point )
	{
		return (plane.Normal.x * point.x) + (plane.Normal.y * point.y) + (plane.Normal.z * point.z) + (plane.D * point.w);
	}
	
	float Plane::DotCoordinate( Plane plane, Vector3 point )
	{
		return (plane.Normal.x * point.x) + (plane.Normal.y * point.y) + (plane.Normal.z * point.z) + plane.D;
	}
	
	float Plane::DotNormal( Plane plane, Vector3 point )
	{
		return (plane.Normal.x * point.x) + (plane.Normal.y * point.y) + (plane.Normal.z * point.z);
	}
	
	void Plane::Normalize()
	{
		float magnitude = 1.0f / static_cast<float>( Math::Sqrt( (Normal.x * Normal.x) + (Normal.y * Normal.y) + (Normal.z * Normal.z) ) );

		Normal.x *= magnitude;
		Normal.y *= magnitude;
		Normal.z *= magnitude;
		D *= magnitude;
	}

	Plane Plane::Normalize( Plane plane )
	{
		float magnitude = 1.0f / static_cast<float>( Math::Sqrt( (plane.Normal.x * plane.Normal.x) + (plane.Normal.y * plane.Normal.y) + (plane.Normal.z * plane.Normal.z) ) );

		return Plane( plane.Normal.x * magnitude, plane.Normal.y * magnitude, plane.Normal.z * magnitude, plane.D * magnitude );
	}

	void Plane::Normalize( Plane% plane, [Out] Plane% result )
	{
		float magnitude = 1.0f / static_cast<float>( Math::Sqrt( (plane.Normal.x * plane.Normal.x) + (plane.Normal.y * plane.Normal.y) + (plane.Normal.z * plane.Normal.z) ) );

		result = Plane( plane.Normal.x * magnitude, plane.Normal.y * magnitude, plane.Normal.z * magnitude, plane.D * magnitude );
	}
	
	Plane Plane::Transform( Plane plane, Matrix transformation )
	{
		Plane result;
		float x = plane.Normal.x;
		float y = plane.Normal.y;
		float z = plane.Normal.z;
		float d = plane.D;

		transformation.Invert();
		result.Normal.x = (((x * transformation.M11) + (y * transformation.M12)) + (z * transformation.M13)) + (d * transformation.M14);
		result.Normal.y = (((x * transformation.M21) + (y * transformation.M22)) + (z * transformation.M23)) + (d * transformation.M24);
		result.Normal.z = (((x * transformation.M31) + (y * transformation.M32)) + (z * transformation.M33)) + (d * transformation.M34);
		result.D		= (((x * transformation.M41) + (y * transformation.M42)) + (z * transformation.M43)) + (d * transformation.M44);

		return result;
	}

	void Plane::Transform( Plane% plane, Matrix% temp, [Out] Plane% result )
	{
		float x = plane.Normal.x;
		float y = plane.Normal.y;
		float z = plane.Normal.z;
		float d = plane.D;

		Matrix transformation = Matrix::Invert( temp );

		Plane r;
		r.Normal.x = (((x * transformation.M11) + (y * transformation.M12)) + (z * transformation.M13)) + (d * transformation.M14);
		r.Normal.y = (((x * transformation.M21) + (y * transformation.M22)) + (z * transformation.M23)) + (d * transformation.M24);
		r.Normal.z = (((x * transformation.M31) + (y * transformation.M32)) + (z * transformation.M33)) + (d * transformation.M34);
		r.D		= (((x * transformation.M41) + (y * transformation.M42)) + (z * transformation.M43)) + (d * transformation.M44);
	
		result = r;
	}

	array<Plane>^ Plane::Transform( array<Plane>^ planes, Matrix% temp )
	{
		if( planes == nullptr )
			throw gcnew ArgumentNullException( "planes" );

		int count = planes->Length;
		array<Plane>^ results = gcnew array<Plane>( count );
		Matrix transformation = Matrix::Invert( temp );

		for( int i = 0; i < count; i++ )
		{
			float x = planes[i].Normal.x;
			float y = planes[i].Normal.y;
			float z = planes[i].Normal.z;
			float d = planes[i].D;

			Plane r;
			r.Normal.x = (((x * transformation.M11) + (y * transformation.M12)) + (z * transformation.M13)) + (d * transformation.M14);
			r.Normal.y = (((x * transformation.M21) + (y * transformation.M22)) + (z * transformation.M23)) + (d * transformation.M24);
			r.Normal.z = (((x * transformation.M31) + (y * transformation.M32)) + (z * transformation.M33)) + (d * transformation.M34);
			r.D		= (((x * transformation.M41) + (y * transformation.M42)) + (z * transformation.M43)) + (d * transformation.M44);
		
			results[i] = r;
		}

		return results;
	}

	Plane Plane::Transform( Plane plane, Quaternion rotation )
	{
		Plane result;
		float x2 = rotation.x + rotation.x;
		float y2 = rotation.y + rotation.y;
		float z2 = rotation.z + rotation.z;
		float wx = rotation.w * x2;
		float wy = rotation.w * y2;
		float wz = rotation.w * z2;
		float xx = rotation.x * x2;
		float xy = rotation.x * y2;
		float xz = rotation.x * z2;
		float yy = rotation.y * y2;
		float yz = rotation.y * z2;
		float zz = rotation.z * z2;

		float x = plane.Normal.x;
		float y = plane.Normal.y;
		float z = plane.Normal.z;

		result.Normal.x = ((x * ((1.0f - yy) - zz)) + (y * (xy - wz))) + (z * (xz + wy));
		result.Normal.y = ((x * (xy + wz)) + (y * ((1.0f - xx) - zz))) + (z * (yz - wx));
		result.Normal.z = ((x * (xz - wy)) + (y * (yz + wx))) + (z * ((1.0f - xx) - yy));
		result.D = plane.D;
		return result;
	}
	
	void Plane::Transform( Plane% plane, Quaternion% rotation, [Out] Plane% result )
	{
		float x2 = rotation.x + rotation.x;
		float y2 = rotation.y + rotation.y;
		float z2 = rotation.z + rotation.z;
		float wx = rotation.w * x2;
		float wy = rotation.w * y2;
		float wz = rotation.w * z2;
		float xx = rotation.x * x2;
		float xy = rotation.x * y2;
		float xz = rotation.x * z2;
		float yy = rotation.y * y2;
		float yz = rotation.y * z2;
		float zz = rotation.z * z2;

		float x = plane.Normal.x;
		float y = plane.Normal.y;
		float z = plane.Normal.z;

		Plane r;
		r.Normal.x = ((x * ((1.0f - yy) - zz)) + (y * (xy - wz))) + (z * (xz + wy));
		r.Normal.y = ((x * (xy + wz)) + (y * ((1.0f - xx) - zz))) + (z * (yz - wx));
		r.Normal.z = ((x * (xz - wy)) + (y * (yz + wx))) + (z * ((1.0f - xx) - yy));
		r.D = plane.D;

		result = r;
	}

	array<Plane>^ Plane::Transform( array<Plane>^ planes, Quaternion% rotation )
	{
		if( planes == nullptr )
			throw gcnew ArgumentNullException( "planes" );

		int count = planes->Length;
		array<Plane>^ results = gcnew array<Plane>( count );

		float x2 = rotation.x + rotation.x;
		float y2 = rotation.y + rotation.y;
		float z2 = rotation.z + rotation.z;
		float wx = rotation.w * x2;
		float wy = rotation.w * y2;
		float wz = rotation.w * z2;
		float xx = rotation.x * x2;
		float xy = rotation.x * y2;
		float xz = rotation.x * z2;
		float yy = rotation.y * y2;
		float yz = rotation.y * z2;
		float zz = rotation.z * z2;

		for( int i = 0; i < count; i++ )
		{
			float x = planes[i].Normal.x;
			float y = planes[i].Normal.y;
			float z = planes[i].Normal.z;

			Plane r;
			r.Normal.x = ((x * ((1.0f - yy) - zz)) + (y * (xy - wz))) + (z * (xz + wy));
			r.Normal.y = ((x * (xy + wz)) + (y * ((1.0f - xx) - zz))) + (z * (yz - wx));
			r.Normal.z = ((x * (xz - wy)) + (y * (yz + wx))) + (z * ((1.0f - xx) - yy));
			r.D = planes[i].D;

			results[i] = r;
		}

		return results;
	}
	
	//bool Plane::Intersects( Plane plane, Vector3 start, Vector3 end, [Out] Vector3% intersectPoint )
	//{
	//	D3DXVECTOR3 nativePoint;
	//	D3DXVECTOR3* result = D3DXPlaneIntersectLine( &nativePoint, reinterpret_cast<D3DXPLANE*>( &plane ), reinterpret_cast<D3DXVECTOR3*>( &start ), reinterpret_cast<D3DXVECTOR3*>( &end ) );
	//	if( result == NULL )
	//	{
	//		intersectPoint = Vector3( 0, 0, 0 );
	//		return false;
	//	}

	//	intersectPoint = Vector3( nativePoint.x, nativePoint.y, nativePoint.z );
	//	return true;
	//}

	PlaneIntersectionType Plane::Intersects( Plane plane, BoundingBox box )
	{
		Vector3 min;
		Vector3 max;
		max.x = (plane.Normal.x >= 0.0f) ? box.Minimum.x : box.Maximum.x;
		max.y = (plane.Normal.y >= 0.0f) ? box.Minimum.y : box.Maximum.y;
		max.z = (plane.Normal.z >= 0.0f) ? box.Minimum.z : box.Maximum.z;
		min.x = (plane.Normal.x >= 0.0f) ? box.Maximum.x : box.Minimum.x;
		min.y = (plane.Normal.y >= 0.0f) ? box.Maximum.y : box.Minimum.y;
		min.z = (plane.Normal.z >= 0.0f) ? box.Maximum.z : box.Minimum.z;

		float dot = (plane.Normal.x * max.x) + (plane.Normal.y * max.y) + (plane.Normal.z * max.z);

		if( dot + plane.D > 0.0f )
			return PlaneIntersectionType::Front;

		dot = (plane.Normal.x * min.x) + (plane.Normal.y * min.y) + (plane.Normal.z * min.z);

		if ( dot + plane.D < 0.0f)
			return PlaneIntersectionType::Back;

		return PlaneIntersectionType::Intersecting;
	}

	PlaneIntersectionType Plane::Intersects( Plane plane, BoundingSphere sphere )
	{
		float dot = (sphere.Center.x * plane.Normal.x) + (sphere.Center.y * plane.Normal.y) + (sphere.Center.z * plane.Normal.z) + plane.D;
		
		if( dot > sphere.Radius )
			return PlaneIntersectionType::Front;

		if( dot < -sphere.Radius )
			return PlaneIntersectionType::Back;

		return PlaneIntersectionType::Intersecting;
	}

	//Plane Plane::Multiply( Plane plane, float scale )
	//{
	//	Plane result;
	//	D3DXPlaneScale( reinterpret_cast<D3DXPLANE*>( &result ), reinterpret_cast<D3DXPLANE*>( &plane ), scale );
	//	return result;
	//}

	//void Plane::Multiply( Plane% plane, float scale, [Out] Plane% result )
	//{
	//	Plane r;
	//	pin_ptr<Plane> pinnedPlane = &plane;
	//	D3DXPlaneScale( reinterpret_cast<D3DXPLANE*>( &r ), reinterpret_cast<D3DXPLANE*>( pinnedPlane ), scale );
	//	result = r;
	//}

	//
	//Plane Plane::operator * ( Plane plane, float scale )
	//{
	//	Plane result;
	//	D3DXPlaneScale( reinterpret_cast<D3DXPLANE*>( &result ), reinterpret_cast<D3DXPLANE*>( &plane ), scale );
	//	return result;
	//}

	//Plane Plane::operator * ( float scale, Plane plane )
	//{
	//	return plane * scale;
	//}

	bool Plane::operator == ( Plane left, Plane right )
	{
		return Plane::Equals( left, right );
	}

	bool Plane::operator != ( Plane left, Plane right )
	{
		return !Plane::Equals( left, right );
	}

	String^ Plane::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "Normal:{0} D:{1}", Normal.ToString(), D.ToString(CultureInfo::CurrentCulture) );
	}

	int Plane::GetHashCode()
	{
		return Normal.GetHashCode() + D.GetHashCode();
	}

	bool Plane::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Plane>( value ) );
	}

	bool Plane::Equals( Plane value )
	{
		return ( Normal == value.Normal && D == value.D );
	}

	bool Plane::Equals( Plane% value1, Plane% value2 )
	{
		return ( value1.Normal == value2.Normal && value1.D == value2.D );
	}
}
