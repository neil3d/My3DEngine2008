/**
*	nexus core - math.color
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_COLOR_H_
#define _NEXUS_COLOR_H_

namespace nexus
{
	/** color */
	struct color4f
	{
		float R,G,B,A;

		color4f(void):R(0),G(0),B(0),A(1)
		{}

		color4f(float r, float g, float b, float a):R(r),G(g),B(b),A(a)
		{}

		/*
		To calculate luminance from RGB values, use this:
		L = 0.27R + 0.67G + 0.06B;
		*/
		float cal_lum()
		{
			return 0.27f*R + 0.67f*G + 0.06f*B;
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

}// namespace nexus

#endif //_NEXUS_COLOR_H_