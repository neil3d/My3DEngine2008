#include "stdafx.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include "Ray.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Ray::Ray( Vector3 position, Vector3 direction )
	{
		Position = position;
		Direction = direction;
	}

	bool Ray::Intersects( Ray ray, Plane plane, [Out] float% distance )
	{
		float dotDirection = (plane.Normal.x * ray.Direction.x) + (plane.Normal.y * ray.Direction.y) + (plane.Normal.z * ray.Direction.z);

		if( Math::Abs( dotDirection ) < 0.000001f )
		{
			distance = 0;
			return false;
		}

		float dotPosition = (plane.Normal.x * ray.Position.x) + (plane.Normal.y * ray.Position.y) + (plane.Normal.z * ray.Position.z);
		float num = ( -plane.D - dotPosition ) / dotDirection;

		if( num < 0.0f )
		{
			if( num < -0.000001f )
			{
				distance = 0;
				return false;
			}
			num = 0.0f;
		}

		distance = num;
		return true;
	}

	//bool Ray::Intersects( Ray ray, Vector3 vertex1, Vector3 vertex2, Vector3 vertex3, [Out] float% distance )
	//{
	//	FLOAT u, v;
	//	pin_ptr<float> pinnedDist = &distance;

	//	if( D3DXIntersectTri( reinterpret_cast<D3DXVECTOR3*>( &vertex1 ), 
	//		reinterpret_cast<D3DXVECTOR3*>( &vertex2 ), reinterpret_cast<D3DXVECTOR3*>( &vertex3 ),
	//		reinterpret_cast<D3DXVECTOR3*>( &ray.Position ), reinterpret_cast<D3DXVECTOR3*>( &ray.Direction ),
	//		&u, &v, reinterpret_cast<FLOAT*>( pinnedDist ) ) )
	//		return true;
	//	else
	//		return false;
	//}

	bool Ray::Intersects( Ray ray, BoundingBox box, [Out] float% distance )
	{
		float d = 0.0f;
		float maxValue = float::MaxValue;

		if( Math::Abs( ray.Direction.x ) < 0.0000001 )
		{
			if( ray.Position.x < box.Minimum.x || ray.Position.x > box.Maximum.x )
			{
				distance = 0.0f;
				return false;
			}
		}
		else
		{
			float inv = 1.0f / ray.Direction.x;
			float min = (box.Minimum.x - ray.Position.x) * inv;
			float max = (box.Maximum.x - ray.Position.x) * inv;

			if( min > max )
			{
				float temp = min;
				min = max;
				max = temp;
			}

			d = Math::Max( min, d );
			maxValue = Math::Min( max, maxValue );

			if( d > maxValue )
			{
				distance = 0.0f;
				return false;
			}
		}

		if( Math::Abs( ray.Direction.y ) < 0.0000001 )
		{
			if( ray.Position.y < box.Minimum.y || ray.Position.y > box.Maximum.y )
			{
				distance = 0.0f;
				return false;
			}
		}
		else
		{
			float inv = 1.0f / ray.Direction.y;
			float min = (box.Minimum.y - ray.Position.y) * inv;
			float max = (box.Maximum.y - ray.Position.y) * inv;

			if( min > max )
			{
				float temp = min;
				min = max;
				max = temp;
			}

			d = Math::Max( min, d );
			maxValue = Math::Min( max, maxValue );

			if( d > maxValue )
			{
				distance = 0.0f;
				return false;
			}
		}

		if( Math::Abs( ray.Direction.z ) < 0.0000001 )
		{
			if( ray.Position.z < box.Minimum.z || ray.Position.z > box.Maximum.z )
			{
				distance = 0.0f;
				return false;
			}
		}
		else
		{
			float inv = 1.0f / ray.Direction.z;
			float min = (box.Minimum.z - ray.Position.z) * inv;
			float max = (box.Maximum.z - ray.Position.z) * inv;

			if( min > max )
			{
				float temp = min;
				min = max;
				max = temp;
			}

			d = Math::Max( min, d );
			maxValue = Math::Min( max, maxValue );

			if( d > maxValue )
			{
				distance = 0.0f;
				return false;
			}
		}

		distance = d;
		return true;
	}

	bool Ray::Intersects( Ray ray, BoundingSphere sphere, [Out] float% distance )
	{
		float x = sphere.Center.x - ray.Position.x;
		float y = sphere.Center.y - ray.Position.y;
		float z = sphere.Center.z - ray.Position.z;
		float pyth = (x * x) + (y * y) + (z * z);
		float rr = sphere.Radius * sphere.Radius;

		if( pyth <= rr )
		{
			distance = 0.0f;
			return true;
		}

		float dot = (x * ray.Direction.x) + (y * ray.Direction.y) + (z * ray.Direction.z);
		if( dot < 0.0f )
		{
			distance = 0.0f;
			return false;
		}

		float temp = pyth - (dot * dot);
		if( temp > rr )
		{
			distance = 0.0f;
			return false;
		}

		distance = dot - static_cast<float>( Math::Sqrt( static_cast<double>( rr - temp ) ) );
		return true;
	}

	bool Ray::operator == ( Ray left, Ray right )
	{
		return Ray::Equals( left, right );
	}

	bool Ray::operator != ( Ray left, Ray right )
	{
		return !Ray::Equals( left, right );
	}

	String^ Ray::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "Position:{0} Direction:{1}", Position.ToString(), Direction.ToString() );
	}

	int Ray::GetHashCode()
	{
		return Position.GetHashCode() + Direction.GetHashCode();
	}

	bool Ray::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Ray>( value ) );
	}

	bool Ray::Equals( Ray value )
	{
		return ( Position == value.Position && Direction == value.Direction );
	}

	bool Ray::Equals( Ray% value1, Ray% value2 )
	{
		return ( value1.Position == value2.Position && value1.Direction == value2.Direction );
	}
}