/**
*	nexus core - math.color
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_COLOR_H_
#define _NEXUS_COLOR_H_
#include "../is_pod.h"

namespace nexus
{
	typedef unsigned int argb_t; 

	/** color */
	struct color4f
	{
		float R,G,B,A;

		color4f(void):R(0),G(0),B(0),A(1)
		{}

		color4f(float r, float g, float b, float a):R(r),G(g),B(b),A(a)
		{}
		color4f(float* data):R(data[0]),G(data[1]),B(data[2]),A(data[3])
		{}

		color4f(argb_t color)
		{
			B = (color & 0xFF) / 255.f; 
			G = (color>>8  & 0xFF) / 255.f; 
			R = (color>>16 & 0xFF) / 255.f; 
			A = (color>>24 & 0xFF) / 255.f; 
		}

		/*
		To calculate luminance from RGB values, use this:
		L = 0.27R + 0.67G + 0.06B;
		*/
		float cal_lum()
		{
			return 0.27f*R + 0.67f*G + 0.06f*B;
		}

		argb_t to_argb()
		{
			return ( static_cast<argb_t>(A * 255) << 24 |
					 static_cast<argb_t>(R * 255) << 16 |
					 static_cast<argb_t>(G * 255) << 8 |
					 static_cast<argb_t>(B * 255)			);
		}
	};

	struct color4ub
	{
		unsigned char R, G, B, A;

		color4ub(void):R(0),G(0),B(0),A(1)
		{}

		color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a):R(r),G(g),B(b),A(a)
		{}

	};

	template<> struct IS_POD<color4f>
	{ 
		static const bool value = true; 
	};

	template<> struct IS_POD<color4ub>
	{ 
		static const bool value = true; 
	};
}// namespace nexus

#endif //_NEXUS_COLOR_H_