/**
*	Core of Nexus Engine -  Const math values
*
*	Author: Neil
*	Date:	Dec, 2008
*/

#ifndef _NEXUS_MATH_CONST_H_
#define _NEXUS_MATH_CONST_H_

namespace nexus
{
	const float nHalfPI		=	float(3.14159265358979323846264338327950288419716939937510582 * 0.5);
	const float nQuarterPI	=	float(3.14159265358979323846264338327950288419716939937510582 * 0.25);
	const float nPI			=	float(3.14159265358979323846264338327950288419716939937510582);
	const float nTwoPI		=	float(3.14159265358979323846264338327950288419716939937510582 * 2.0);

	const float nEPS		=	float(10e-6);
	const float nDoubleEPS	=	float(10e-6) * 2;
	const float nBigEPS		=	float(10e-2);
	const float	nSmallEPS	=	float(10e-6);

	const float WORLD_MAX		=	524288.0;	/* Maximum size of the world */
	const float HALF_WORLD_MAX	=	262144.0;	/* Half the maximum size of the world */
	const float HALF_WORLD_MAX1	=	262143.0;	/* Half the maximum size of the world - 1*/

	inline float nRadToDeg(float fRadians)
	{
		return fRadians * 57.2957795f;
	}


	/////////////////////////////////////////////////////////////////////////////
	//  DegToRad

	inline float nDegToRad(float fDegrees)
	{
		return fDegrees / 57.2957795f;
	}

	// fuzzy logic zero
	template <class Type>
	inline bool is_zero( Type value ) 
	{
		return ( fabs( value ) < nEPS );
	}

	template<typename T>
	inline T linear_lerp(const T& a, const T& b, const T& alpha)
	{
		return (T)(a + alpha*(b-a));
	}

	template<typename T>
	inline T quad_lerp(const T& p00, const T& p10, const T& p01, const T& p11,
		float u, float v)
	{
		if( u > v )
		{
			if( v<1.0f )
				return linear_lerp( linear_lerp(p00, p11, v),	linear_lerp(p10, p11, v),
				(u-v)/(1.0f-v)	);
			else
				return p11;
		}
		else
		{
			if( v>0.0f )
				return linear_lerp( linear_lerp(p00, p01, v), linear_lerp(p00, p11, v),
				u/v );
			else
				return p00;
		}
	}

	template< class Type >
	inline Type clamp( const Type x, const Type min, const Type max )
	{
		return x<min ? min : x<max ? x : max;
	}

	template< class T, class U > T lerp( const T& a, const T& b, const U& alpha )
	{
		return (T)(a + alpha * (b-a));
	}

	// square
	template <class Type>
	inline Type square( Type value )
	{
		return ( value * value );
	}

	template <typename T>
	inline T inv(T x)
	{
		return T(1) / x;
	}

	// converts to integer equal to or less than (asm version)
	inline int floor( float f ) 
	{
#ifndef _WIN64
		static float Half = 0.5;
		int i; 
		__asm fld [f]
		__asm fsub [Half]
		__asm fistp [i]
		return i;
#else
		return int(f);
#endif
	}

	//  round (to nearest) a floating point number to an integer.
	inline int round( float f )
	{
#ifndef _WIN64
		int retval;
		__asm fld [f]
		__asm fistp [retval]
		return retval;
#else
		return int(f+0.5f);
#endif
	}


	// sinus & cosinus
	inline void sin_cos( float inAngle, float &outSin, float &outCos ) 
	{
#ifndef _WIN64
		__asm 
		{
			fld inAngle 
				fsincos 
				mov edi, outCos 
				fstp dword ptr [edi] 
			mov edi, outSin 
				fstp dword ptr [edi] 
		}
#else
		outSin = sin(inAngle);
		outCos = cos(inAngle);
#endif
	}

	/**
	* Snaps a value to the nearest grid multiple.
	*/
	inline float snap( float location, float grid )
	{
		if( grid==0.f )	return location;
		else			return floor((location + 0.5f*grid)/grid)*grid;
	}

}//namespace nexus

#endif //_NEXUS_MATH_CONST_H_