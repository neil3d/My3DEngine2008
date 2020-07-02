/**
*	nexus core - math
*
*	Author: Neil
*	Date:	Jan, 2008
*/

#ifndef _NEXUS_VECTOR_H_
#define _NEXUS_VECTOR_H_
#include "math_const.h"

namespace nexus
{
	//-- vector define
	/** vector2 */
	struct vector2
	{
		float x, y;

		vector2(void):x(0),y(0)	{}
		vector2(float _x, float _y):x(_x),y(_y)	{}

		const vector2& operator += (const vector2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		const vector2& operator -= (const vector2& v)
		{
			x -= v.x;
			y -= v.y;			
			return *this;
		}

		const vector2& operator *= (float v)
		{
			x *= v;
			y *= v;			
			return *this;
		}
	};

	/** vector3 */
	struct vector3
	{
		float x, y, z;

		vector3(void):x(0),y(0),z(0)	{}
		vector3(float _x, float _y, float _z): x(_x),y(_y),z(_z)	{}

		const vector3& operator += (const vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		const vector3& operator -= (const vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		const vector3& operator *= (float v)
		{
			x *= v;
			y *= v;
			z *= v;
			return *this;
		}

		const vector3& operator *= (const vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		// unary operators
		vector3 operator - () const
		{
			return vector3(-x, -y, -z);
		}
	};

	/** vector4 */
	struct vector4
	{
		float x, y, z, w;

		vector4(void):x(0),y(0),z(0),w(0)	{}
		vector4(float _x, float _y, float _z, float _w): x(_x),y(_y),z(_z),w(_w)	{}

		const vector4& operator += (const vector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		const vector4& operator -= (const vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
	};


	//-- vector2相关计算 --------------------------------------------------------
	/** vector2 add */
	inline vector2 operator + (vector2 lv, vector2 rv)
	{
		return vector2(lv.x+rv.x, lv.y+rv.y);
	}

	/** vector2 subtract*/
	inline vector2 operator - (vector2 lv, vector2 rv)
	{
		return vector2(lv.x-rv.x, lv.y-rv.y);
	}

	/** vector2 scale */
	inline vector2 operator * (vector2 lv, float s)
	{
		return vector2(lv.x*s, lv.y*s);
	}

	/** vector2 length */
	inline float vec_length(const vector2& v)
	{
		return sqrt(v.x*v.x + v.y*v.y);
	}

	inline float vec_length_sq(const vector2& v)
	{
		return (v.x*v.x + v.y*v.y);
	}

	/** vector2 normalize */
	inline vector2 vec_normalize(const vector2& v)
	{
		float m = 1.0f/vec_length(v);
		return vector2(v.x * m, v.y * m);		
	}

	//-- vector3相关计算 --------------------------------------------------------
	/** vector3 add */
	inline vector3 operator + (vector3 lv, vector3 rv)
	{
		return vector3(lv.x+rv.x, lv.y+rv.y, lv.z+rv.z);
	}

	/** vector3 subtract*/
	inline vector3 operator - (vector3 lv, vector3 rv)
	{
		return vector3(lv.x-rv.x, lv.y-rv.y, lv.z-rv.z);
	}

	/** vector3 scale */
	inline vector3 operator * (vector3 lv, float s)
	{
		return vector3(lv.x*s, lv.y*s, lv.z*s);
	}

	/** vector3 length */
	inline float vec_length(const vector3& v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	inline float vec_length_sq(const vector3& v)
	{
		return (v.x*v.x + v.y*v.y + v.z*v.z);
	}

	inline float vec_distance(const vector3& p1, const vector3& p2)
	{
		return vec_length(p1-p2);
	}

	inline float vec_distance_sq(const vector3& p1, const vector3& p2)
	{
		return vec_length_sq(p1-p2);
	}

	/** vector3 normalize */
	inline vector3 vec_normalize(const vector3& v)
	{
		float m = vec_length(v);
		if( m > nEPS )
			m = 1.0f/m;
		else
			m = 0;
		
		return vector3(v.x * m,
			v.y * m,
			v.z * m);
	}

	/** vector3 dot*/
	inline float vec_dot(const vector3& lv, const vector3& rv)
	{
		return lv.x*rv.x + lv.y*rv.y + lv.z*rv.z;
	}

	/** vector3 cross*/
	inline vector3 vec_cross(const vector3& lv, const vector3& rv)
	{
		return vector3(lv.y * rv.z - lv.z * rv.y,
			lv.z * rv.x - lv.x * rv.z,
			lv.x * rv.y - lv.y * rv.x);
	}
}//namespace nexus
#endif //_NEXUS_VECTOR_H_