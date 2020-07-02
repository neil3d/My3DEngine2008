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

	template <class Type>
	inline Type linear_lerp( const Type &a, const Type &b, const float r ) 
	{
		return a * (1.0f - r) + b * r;
	}

	// sqare
	template <class Type>
	inline Type sqare( Type value )
	{
		return ( value * value );
	}

	// converts to integer equal to or less than (asm version)
	inline int floor( float f ) 
	{
		static float Half = 0.5;
		int i; 
		__asm fld [f]
		__asm fsub [Half]
		__asm fistp [i]
		return i;
	}

	// sinus & cosinus
	inline void sin_cos( float inAngle, float &outSin, float &outCos ) 
	{
		__asm 
		{
			fld inAngle 
				fsincos 
				mov edi, outCos 
				fstp dword ptr [edi] 
			mov edi, outSin 
				fstp dword ptr [edi] 
		}
	}

}//namespace nexus

#endif //_NEXUS_MATH_CONST_H_