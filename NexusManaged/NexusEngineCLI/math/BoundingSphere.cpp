#include "stdafx.h"
//#include <d3dx9.h>
//#include "../SlimDXException.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "Plane.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	BoundingSphere::BoundingSphere( Vector3 center, float radius )
	{
		Center = center;
		Radius = radius;
	}

	ContainmentType BoundingSphere::Contains( BoundingSphere sphere, BoundingBox box )
	{
		Vector3 vector;

		if( !BoundingBox::Intersects( box, sphere ) )
			return ContainmentType::Disjoint;

		float radius = sphere.Radius * sphere.Radius;
		vector.x = sphere.Center.x - box.Minimum.x;
		vector.y = sphere.Center.y - box.Maximum.y;
		vector.z = sphere.Center.z - box.Maximum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Maximum.x;
		vector.y = sphere.Center.y - box.Maximum.y;
		vector.z = sphere.Center.z - box.Maximum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Maximum.x;
		vector.y = sphere.Center.y - box.Minimum.y;
		vector.z = sphere.Center.z - box.Maximum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Minimum.x;
		vector.y = sphere.Center.y - box.Minimum.y;
		vector.z = sphere.Center.z - box.Maximum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Minimum.x;
		vector.y = sphere.Center.y - box.Maximum.y;
		vector.z = sphere.Center.z - box.Minimum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Maximum.x;
		vector.y = sphere.Center.y - box.Maximum.y;
		vector.z = sphere.Center.z - box.Minimum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Maximum.x;
		vector.y = sphere.Center.y - box.Minimum.y;
		vector.z = sphere.Center.z - box.Minimum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		vector.x = sphere.Center.x - box.Minimum.x;
		vector.y = sphere.Center.y - box.Minimum.y;
		vector.z = sphere.Center.z - box.Minimum.z;

		if( vector.LengthSquared() > radius )
			return ContainmentType::Intersects;

		return ContainmentType::Contains;
	}

	ContainmentType BoundingSphere::Contains( BoundingSphere sphere1, BoundingSphere sphere2 )
	{
		float distance;
		float x = sphere1.Center.x - sphere2.Center.x;
		float y = sphere1.Center.y - sphere2.Center.y;
		float z = sphere1.Center.z - sphere2.Center.z;

		distance = static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) ) );
		float radius = sphere1.Radius;
		float radius2 = sphere2.Radius;

		if( radius + radius2 < distance )
			return ContainmentType::Disjoint;

		if( radius - radius2 < distance )
			return ContainmentType::Intersects;

		return ContainmentType::Contains;
	}

	ContainmentType BoundingSphere::Contains( BoundingSphere sphere, Vector3 vector )
	{
		float x = vector.x - sphere.Center.x;
		float y = vector.y - sphere.Center.y;
		float z = vector.z - sphere.Center.z;

		float distance = (x * x) + (y * y) + (z * z);

		if( distance >= (sphere.Radius * sphere.Radius) )
			return ContainmentType::Disjoint;

		return ContainmentType::Contains;
	}

	BoundingSphere BoundingSphere::FromBox( BoundingBox box )
	{
		BoundingSphere sphere;
		Vector3::Lerp( box.Minimum, box.Maximum, 0.5f, sphere.Center );

		float x = box.Minimum.x - box.Maximum.x;
		float y = box.Minimum.y - box.Maximum.y;
		float z = box.Minimum.z - box.Maximum.z;

		float distance = static_cast<float>( Math::Sqrt( (x * x) + (y * y) + (z * z) ) );

		sphere.Radius = distance * 0.5f;

		return sphere;
	}

	//BoundingSphere BoundingSphere::FromPoints( array<Vector3>^ points )
	//{
	//	D3DXVECTOR3 center;
	//	FLOAT radius;
	//	pin_ptr<Vector3> pinnedPoints = &points[0];

	//	HRESULT hr = D3DXComputeBoundingSphere( reinterpret_cast<const D3DXVECTOR3*>( pinnedPoints ), points->Length, sizeof(float) * 3, &center, &radius );
	//	if( RECORD_SDX( hr ).IsFailure )
	//		return BoundingSphere();

	//	BoundingSphere sphere;
	//	sphere.Center = Vector3( center.x, center.y, center.z );
	//	sphere.Radius = radius;

	//	return sphere;
	//}

	BoundingSphere BoundingSphere::Merge( BoundingSphere sphere1, BoundingSphere sphere2 )
	{
		BoundingSphere sphere;
		Vector3 difference = sphere2.Center - sphere1.Center;

		float length = difference.Length();
		float radius = sphere1.Radius;
		float radius2 = sphere2.Radius;

		if( radius + radius2 >= length)
		{
			if( radius - radius2 >= length )
				return sphere1;

			if( radius2 - radius >= length )
				return sphere2;
		}

		Vector3 vector = difference * ( 1.0f / length );
		float min = Math::Min( -radius, length - radius2 );
		float max = ( Math::Max( radius, length + radius2 ) - min ) * 0.5f;

		sphere.Center = sphere1.Center + vector * ( max + min );
		sphere.Radius = max;

		return sphere;
	}

	bool BoundingSphere::Intersects( BoundingSphere sphere, BoundingBox box )
	{
		return BoundingBox::Intersects( box, sphere );
	}

	bool BoundingSphere::Intersects( BoundingSphere sphere1, BoundingSphere sphere2 )
	{
		float distance;
		distance = Vector3::DistanceSquared( sphere1.Center, sphere2.Center );
		float radius = sphere1.Radius;
		float radius2 = sphere2.Radius;

		if( (radius * radius) + (2.0f * radius * radius2) + (radius2 * radius2) <= distance )
			return false;

		return true;
	}

	bool BoundingSphere::Intersects( BoundingSphere sphere, Ray ray, [Out] float% distance )
	{
		return Ray::Intersects( ray, sphere, distance );
	}

	PlaneIntersectionType BoundingSphere::Intersects( BoundingSphere sphere, Plane plane )
	{
		return Plane::Intersects( plane, sphere );
	}

	bool BoundingSphere::operator == ( BoundingSphere left, BoundingSphere right )
	{
		return BoundingSphere::Equals( left, right );
	}

	bool BoundingSphere::operator != ( BoundingSphere left, BoundingSphere right )
	{
		return !BoundingSphere::Equals( left, right );
	}

	String^ BoundingSphere::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "Center:{0} Radius:{1}", Center.ToString(), Radius.ToString(CultureInfo::CurrentCulture) );
	}

	int BoundingSphere::GetHashCode()
	{
		return Center.GetHashCode() + Radius.GetHashCode();
	}

	bool BoundingSphere::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<BoundingSphere>( value ) );
	}

	bool BoundingSphere::Equals( BoundingSphere value )
	{
		return ( Center == value.Center && Radius == value.Radius );
	}

	bool BoundingSphere::Equals( BoundingSphere% value1, BoundingSphere% value2 )
	{
		return ( value1.Center == value2.Center && value1.Radius == value2.Radius );
	}
}
