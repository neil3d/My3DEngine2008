/**
*	nexus core - math
*
*	Author: Neil
*	Date:	Jan, 2008
*/

#ifndef _NEXUS_QUAT_H_
#define _NEXUS_QUAT_H_
#include "../base_define.h"
#include "vector.h"
#include "matrix.h"

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201) // anonymous unions warning

namespace nexus
{
	/** ËÄÔªÊý */
	struct nCoreAPI quat
	{
		union
		{
			float value[4];
			struct
			{
				float x, y, z, w;
			};	
		};

		quat(void):x(0),y(0),z(0),w(1)	{}
		quat(float _x, float _y, float _z, float _w): x(_x),y(_y),z(_z),w(_w)	{}
		
		explicit quat(const vector4& i) : x(i.x),y(i.y),z(i.z),w(i.w) {}
		
		quat(const quat& rq) : x(rq.x),y(rq.y),z(rq.z),w(rq.w) {}

		operator const float* () const { return &(value[0]); }
		operator float* () { return &(value[0]); }	

		inline quat& operator= (const quat& rq)
		{
			w = rq.w;
			x = rq.x;
			y = rq.y;
			z = rq.z;
			return *this;
		}
		quat operator+ (const quat& rq) const;
		quat operator- (const quat& rq) const;
		quat operator* (const quat& rq) const;
		quat operator* (float fScalar) const;
		friend quat operator* (float fScalar, const quat& rq);
		quat operator /(const quat &q) const;
		quat operator /(float scale) const;
		quat operator- () const;
		bool operator== (const quat& rhs) const;
		bool operator!= (const quat& rhs) const;

		// special values
		static const quat zero;
		static const quat identity;
	};

	nCoreAPI void	quat_set_identity(quat& q);
	nCoreAPI quat	quat_inverse(const quat& q);
	nCoreAPI quat	quat_normalize(const quat& q);
	nCoreAPI float	quat_dot(const quat& q1, const quat& q2);
	nCoreAPI quat	quat_slerp(const quat& q1, const quat& q2, float s);
	nCoreAPI void	quat_set_rotation(quat& q, const vector3& axis, float angle);
	nCoreAPI void	quat_to_matrix33(const quat& q, matrix33& mat);
	nCoreAPI void	quat_to_matrix44(const quat& q, matrix44& mat);
	nCoreAPI quat matrix44_to_quat(const matrix44& m);
	nCoreAPI quat matrix33_to_quat(const matrix33& m);

	// the angle is in radians
	nCoreAPI quat quat_from_axis_angle(const vector3& axis, const float a);
	// the angle is in radians
	nCoreAPI quat quat_from_axis_angle(const float x, const float y, const float z, const float angle);
	// the angle is stored in radians
	nCoreAPI void quat_to_axis_angle(const quat& qq, vector3* axis, float *angle);

	// Spherical linear interpolation
	nCoreAPI quat quat_slerp(const quat& qq1, const quat& qq2, const float t);
	// Sperical quadtratic interpolation using a smooth cubic spline
	// The parameters a and b are the control points.
	nCoreAPI quat quat_squad(const quat& q0, 
		const quat& a, 
		const quat& b, 
		const quat& q1, 
		const float t);
	

	template<> struct IS_POD<quat>
	{ 
		static const bool value = true; 
	};

}//namespace nexus


#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4201)
#endif

#endif //_NEXUS_QUAT_H_