#include "stdafx.h"
//#include "../SlimDXException.h"
//#include "../DataStream.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Ray.h"
#include "Plane.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	BoundingBox::BoundingBox( Vector3 minimum, Vector3 maximum )
	{
		Minimum = minimum;
		Maximum = maximum;
	}

	array<Vector3>^ BoundingBox::GetCorners()
	{
		array<Vector3>^ results = gcnew array<Vector3>( 8 );
		results[0] = Vector3(Minimum.x, Maximum.y, Maximum.z);
		results[1] = Vector3(Maximum.x, Maximum.y, Maximum.z);
		results[2] = Vector3(Maximum.x, Minimum.y, Maximum.z);
		results[3] = Vector3(Minimum.x, Minimum.y, Maximum.z);
		results[4] = Vector3(Minimum.x, Maximum.y, Minimum.z);
		results[5] = Vector3(Maximum.x, Maximum.y, Minimum.z);
		results[6] = Vector3(Maximum.x, Minimum.y, Minimum.z);
		results[7] = Vector3(Minimum.x, Minimum.y, Minimum.z);
		return results;
	}

	ContainmentType BoundingBox::Contains( BoundingBox box1, BoundingBox box2 )
	{
		if( box1.Maximum.x < box2.Minimum.x || box1.Minimum.x > box2.Maximum.x )
			return ContainmentType::Disjoint;

		if( box1.Maximum.y < box2.Minimum.y || box1.Minimum.y > box2.Maximum.y )
			return ContainmentType::Disjoint;

		if( box1.Maximum.z < box2.Minimum.z || box1.Minimum.z > box2.Maximum.z )
			return ContainmentType::Disjoint;

		if( box1.Minimum.x <= box2.Minimum.x && box2.Maximum.x <= box1.Maximum.x && box1.Minimum.y <= box2.Minimum.y && 
			box2.Maximum.y <= box1.Maximum.y && box1.Minimum.z <= box2.Minimum.z && box2.Maximum.z <= box1.Maximum.z )
			return ContainmentType::Contains;

		return ContainmentType::Intersects;
	}

	ContainmentType BoundingBox::Contains( BoundingBox box, BoundingSphere sphere )
	{
		float dist;
		Vector3 clamped;

		Vector3::Clamp( sphere.Center, box.Minimum, box.Maximum, clamped );

		float x = sphere.Center.x - clamped.x;
		float y = sphere.Center.y - clamped.y;
		float z = sphere.Center.z - clamped.z;

		dist = (x * x) + (y * y) + (z * z);
		float radius = sphere.Radius;

		if( dist > (radius * radius) )
			return ContainmentType::Disjoint;

		if( box.Minimum.x + radius <= sphere.Center.x && sphere.Center.x <= box.Maximum.x - radius && 
			box.Maximum.x - box.Minimum.x > radius && box.Minimum.y + radius <= sphere.Center.y && 
			sphere.Center.y <= box.Maximum.y - radius && box.Maximum.y - box.Minimum.y > radius && 
			box.Minimum.z + radius <= sphere.Center.z && sphere.Center.z <= box.Maximum.z - radius &&
			box.Maximum.x - box.Minimum.x > radius )
			return ContainmentType::Contains;

		return ContainmentType::Intersects;
	}

	ContainmentType BoundingBox::Contains( BoundingBox box, Vector3 vector )
	{
		if( box.Minimum.x <= vector.x && vector.x <= box.Maximum.x && box.Minimum.y <= vector.y && 
			vector.y <= box.Maximum.y && box.Minimum.z <= vector.z && vector.z <= box.Maximum.z )
			return ContainmentType::Contains;

		return ContainmentType::Disjoint;
	}

	BoundingBox BoundingBox::FromPoints( array<Vector3>^ points )
	{
		if( points == nullptr || points->Length <= 0 )
			throw gcnew ArgumentNullException( "points" );

		Vector3 min = Vector3( float::MaxValue );
		Vector3 max = Vector3( float::MinValue );

		for each( Vector3 vector in points )
		{
			Vector3::Minimize( min, vector, min );
			Vector3::Maximize( max, vector, max );
		}

		return BoundingBox( min, max );
	}

	/*BoundingBox BoundingBox::FromPoints( DataStream^ points, int count, int stride )
	{
		BoundingBox box;

		HRESULT hr = D3DXComputeBoundingBox( reinterpret_cast<D3DXVECTOR3*>( points->PositionPointer ), count, stride, 
			reinterpret_cast<D3DXVECTOR3*>( &box.Minimum ), reinterpret_cast<D3DXVECTOR3*>( &box.Maximum ) );

		if( RECORD_SDX( hr ).IsFailure )
			return BoundingBox();

		return box;
	}*/

	BoundingBox BoundingBox::FromSphere( BoundingSphere sphere )
	{
		BoundingBox box;
		box.Minimum = Vector3( sphere.Center.x - sphere.Radius, sphere.Center.y - sphere.Radius, sphere.Center.z - sphere.Radius );
		box.Maximum = Vector3( sphere.Center.x + sphere.Radius, sphere.Center.y + sphere.Radius, sphere.Center.z + sphere.Radius );
		return box;
	}

	BoundingBox BoundingBox::Merge( BoundingBox box1, BoundingBox box2 )
	{
		BoundingBox box;
		Vector3::Minimize( box1.Minimum, box2.Minimum, box.Minimum );
		Vector3::Maximize( box1.Maximum, box2.Maximum, box.Maximum );
		return box;
	}

	bool BoundingBox::Intersects( BoundingBox box1, BoundingBox box2 )
	{
		if ( box1.Maximum.x < box2.Minimum.x || box1.Minimum.x > box2.Maximum.x )
			return false;

		if ( box1.Maximum.y < box2.Minimum.y || box1.Minimum.y > box2.Maximum.y )
			return false;

		return ( box1.Maximum.z >= box2.Minimum.z && box1.Minimum.z <= box2.Maximum.z );
	}

	bool BoundingBox::Intersects( BoundingBox box, BoundingSphere sphere )
	{
		float dist;
		Vector3 clamped;

		Vector3::Clamp( sphere.Center, box.Minimum, box.Maximum, clamped );

		float x = sphere.Center.x - clamped.x;
		float y = sphere.Center.y - clamped.y;
		float z = sphere.Center.z - clamped.z;

		dist = (x * x) + (y * y) + (z * z);

		return ( dist <= (sphere.Radius * sphere.Radius) );
	}

	bool BoundingBox::Intersects( BoundingBox box, Ray ray, [Out] float% distance )
	{
		return Ray::Intersects( ray, box, distance );
	}

	PlaneIntersectionType BoundingBox::Intersects( BoundingBox box, Plane plane )
	{
		return Plane::Intersects( plane, box );
	}

	bool BoundingBox::operator == ( BoundingBox left, BoundingBox right )
	{
		return BoundingBox::Equals( left, right );
	}

	bool BoundingBox::operator != ( BoundingBox left, BoundingBox right )
	{
		return !BoundingBox::Equals( left, right );
	}

	String^ BoundingBox::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "Minimum:{0} Maximum:{1}", Minimum.ToString(), Maximum.ToString() );
	}

	int BoundingBox::GetHashCode()
	{
		return Minimum.GetHashCode() + Maximum.GetHashCode();
	}

	bool BoundingBox::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<BoundingBox>( value ) );
	}

	bool BoundingBox::Equals( BoundingBox value )
	{
		return ( Minimum == value.Minimum && Maximum == value.Maximum );
	}

	bool BoundingBox::Equals( BoundingBox% value1, BoundingBox% value2 )
	{
		return ( value1.Minimum == value2.Minimum && value1.Maximum == value2.Maximum );
	}
}
