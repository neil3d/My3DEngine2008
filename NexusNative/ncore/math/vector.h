/**
*	nexus core - math
*
*	Author: Neil
*	Date:	Jan, 2008
*/

#ifndef _NEXUS_VECTOR_H_
#define _NEXUS_VECTOR_H_
#include "math_const.h"
#include "../is_pod.h"
#include <xmmintrin.h>

namespace nexus
{
	//-- vector define
	/** vector2 */
	struct vector3;
	struct vector4;

	struct vector2
	{
		float x, y;

		vector2(void):x(0),y(0)	{}
		vector2(float _x, float _y):x(_x),y(_y)	{}
		explicit vector2(float* data):x(data[0]),y(data[1]){}
		//explicit vector2( const vector2& v ):x(v.x),y(v.y)	{}

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
	struct nCoreAPI vector3
	{
		float x, y, z;

		static vector3 zero;
		static vector3 one;

		vector3(void):x(0),y(0),z(0)	{}
		vector3(float _x, float _y, float _z): x(_x),y(_y),z(_z)	{}
		explicit vector3(float* data):x(data[0]),y(data[1]),z(data[2])	{}
		explicit vector3(const vector4& data);
		//explicit vector3( const vector3& v ):x(v.x),y(v.y),z(v.z){}

		inline float operator [] ( unsigned i ) const
		{
			assert( i < 3 );

			return *(&x+i);
		}

		inline float& operator [] ( unsigned i )
		{
			assert( i < 3 );

			return *(&x+i);
		}

		inline vector3& operator = ( const vector3& rk )
		{
			x = rk.x;
			y = rk.y;
			z = rk.z;            

			return *this;
		}

		inline vector3& operator = ( const vector4& rk );

		inline bool operator == ( const vector3& rk ) const
		{
			return ( x == rk.x && y == rk.y && z == rk.z );
		}

		inline bool operator != ( const vector3& rk ) const
		{
			return ( x != rk.x || y != rk.y || z != rk.z );
		}

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

		const vector3& operator /= (float v)
		{
			x /= v;
			y /= v;
			z /= v;
			return *this;
		}
	};

	#pragma pack( push, 16)
	/** vector4 */
	struct vector4
	{
		float x, y, z, w;

		vector4(void):x(0),y(0),z(0),w(0)	{}
		vector4(float _x, float _y, float _z, float _w = 1): x(_x),y(_y),z(_z),w(_w)	{}
		explicit vector4(float* data):x(data[0]),y(data[1]),z(data[2]),w(data[3]){}
		explicit vector4(const vector3& data):x(data.x),y(data.y),z(data.z),w(1) {}

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

		// unary operators
		vector4 operator - () const
		{
			return vector4(-x, -y, -z, -w);
		}

		const vector4& operator *= (float v)
		{
			x *= v;
			y *= v;
			z *= v;
			w *= v;
			return *this;
		}

		const vector4& operator /= (float v)
		{
			x /= v;
			y /= v;
			z /= v;
			w /= v;
			return *this;
		}

		inline vector4& operator = ( const vector3& rk )
		{
			x = rk.x;
			y = rk.y;
			z = rk.z;
			w = 1.0f;

			return *this;
		}
	};
	
	#pragma pack( pop)

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
		float m = vec_length(v);
		if( m > nEPS )
			m = 1.0f/m;
		else
			m = 0;

		return vector2(v.x * m, v.y * m);		
	}

	/** vector2 dot*/
	inline float vec_dot(const vector2& lv, const vector2& rv)
	{
		return lv.x*rv.x + lv.y*rv.y;
	}

	//-- vector3相关计算 --------------------------------------------------------
	/** vector3 add */
	inline vector3 operator + (const vector3& lv, const vector3& rv)
	{
		return vector3(lv.x+rv.x, lv.y+rv.y, lv.z+rv.z);
	}

	/** vector3 subtract*/
	inline vector3 operator - ( const vector3& lv, const vector3& rv)
	{
		return vector3(lv.x-rv.x, lv.y-rv.y, lv.z-rv.z);
	}

	/** vector3 scale */
	inline vector3 operator / ( const vector3& lv, float s)
	{
		const float rsacle = 1/s;
		return vector3(lv.x*rsacle, lv.y*rsacle, lv.z*rsacle);
	}
	inline vector3 operator * ( const vector3& lv, float s)
	{
		return vector3(lv.x*s, lv.y*s, lv.z*s);
	}
	inline vector3 operator * (const vector3& lv, const vector3& s)
	{
		return vector3(lv.x*s.x, lv.y*s.y, lv.z*s.z);
	}
	inline vector3 operator / (const vector3& lv, const vector3& s)
	{
		return vector3(lv.x/s.x, lv.y/s.y, lv.z/s.z);
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

	inline float vec_length_sq2d(const vector3& v)
	{
		return (v.x*v.x + v.z*v.z);
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

	inline vector3 vec_lerp(const vector3& start,const vector3& end, float factor)
	{
		return vector3( start.x + ((end.x - start.x) * factor),
						start.y + ((end.y - start.y) * factor),
						start.z + ((end.z - start.z) * factor) );
	}

	inline float vec_calc_yaw(const vector3& dir )
	{
		vector3 temp = dir;
		temp.y=0;
		temp = vec_normalize(temp);

		float yaw = vec_dot(vector3(0,0,-1), temp);

		if(yaw>1.0f)
			yaw=1.0f;
		if(yaw<-1.0f)
			yaw=-1.0f;

		yaw=acosf(yaw);
		if(temp.x>0.0f)
		{
			yaw = nPI * 2-yaw;
		}
		return yaw;
	}

	/** vector4 length */
	inline float vec_length(const vector4& v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
	}

	/** vector4 normalize */
	inline vector4 vec_normalize(const vector4& v)
	{
		float m = vec_length(v);
		if( m > nEPS )
			m = 1.0f/m;
		else
			m = 0;

		return vector4(v.x * m,
			v.y * m,
			v.z * m,
			v.w * m);
	}

	template<> struct IS_POD<vector2>
	{ 
		static const bool value = true; 
	};

	template<> struct IS_POD<vector3>
	{ 
		static const bool value = true; 
	};

	template<> struct IS_POD<vector4>
	{ 
		static const bool value = true; 
	};

	//!	产生一个随机的单位向量
	inline vector3 vec3_rand_unit()
	{
		vector3 tmp((float)rand()/(RAND_MAX/2.0f)-1.0f,
			(float)rand()/(RAND_MAX/2.0f)-1.0f,
			(float)rand()/(RAND_MAX/2.0f)-1.0f);
		
		return vec_normalize(tmp);
	}
}//namespace nexus
#endif //_NEXUS_VECTOR_H_