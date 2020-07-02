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

namespace nexus
{
	/** ËÄÔªÊý */
	struct quat
	{
		float x, y, z, w;

		quat(void):x(0),y(0),z(0),w(0)	{}
		quat(float _x, float _y, float _z, float _w): x(_x),y(_y),z(_z),w(_w)	{}

		// negative
		quat operator - () const
		{
			return quat(-x, -y, -z, -w);
		}
	};

	nCoreAPI void	quat_set_identity(quat& q);
	nCoreAPI quat	quat_inverse(const quat& q);
	nCoreAPI quat	quat_normalize(const quat& q);
	nCoreAPI float	quat_dot(const quat& q1, const quat& q2);
	nCoreAPI quat	quat_slerp(const quat& q1, const quat& q2, float s);
	nCoreAPI void	quat_set_rotation(quat& q, const vector3& axis, float angle);
	nCoreAPI void	quat_to_matrix(const quat& q, matrix33& mat);
	nCoreAPI void	quat_to_matrix(const quat& q, matrix44& mat);

	inline quat operator*(const quat& p, const quat& q)
	{
		return quat(
			p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
			p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z,
			p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x,
			p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z
			);
	}
}//namespace nexus

#endif //_NEXUS_QUAT_H_