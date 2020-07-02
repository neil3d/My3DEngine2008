#include "stdafx.h"
#include "quat.h"
#include "math_const.h"

namespace nexus
{
	nCoreAPI void quat_set_identity(quat& q)
	{
		q.x = q.y = q.z = 0.0f;
		q.w = 1.0f;
	}

	nCoreAPI quat quat_inverse(const quat& q)
	{
		return quat(-q.x, -q.y, -q.z, q.w);
	}

	nCoreAPI quat quat_normalize(const quat& q)
	{
		float len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
		float inv_len;

		if (len > nEPS) 		
			inv_len = 1 / len;
		else
			inv_len = 0;

		return quat(q.x * inv_len,
			q.y * inv_len,
			q.z * inv_len,
			q.w * inv_len);
	}

	nCoreAPI float	quat_dot(const quat& q1, const quat& q2)
	{
		return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	}

	nCoreAPI quat	quat_slerp(const quat& q1, const quat& q2, float s)
	{
		float cosine = quat_dot(q1, q2);
		if (cosine < -1)
			cosine = -1;
		else if (cosine > 1)
			cosine = 1;

		float angle = (float)acosf(cosine);
		if (fabs(angle) < nEPS) 		
			return q1;			
		
		float sine = sinf(angle);
		float sineInv = 1.0f / sine;
		float c1 = sinf((1.0f - s) * angle) * sineInv;
		float c2 = sinf(s * angle) * sineInv;

		return quat(c1 * q1.x + c2 * q2.x,
			c1 * q1.y + c2 * q2.y,
			c1 * q1.z + c2 * q2.z,
			c1 * q1.w + c2 * q2.w
			);		
	}

	nCoreAPI void	quat_set_rotation(quat& q, const vector3& axis, float angle)
	{
		// axis must be normalized

		float s = sinf(angle/2.0f);
		q.x = s * axis.x;
		q.y = s * axis.y;
		q.z = s * axis.z;
		q.w = cosf(angle/2.0f);		
	}

	nCoreAPI void	quat_to_matrix(const quat& q, matrix33& mat)
	{
		float x2 = q.x * 2;
		float y2 = q.y * 2;
		float z2 = q.z * 2;
		float wx = x2 * q.w;
		float wy = y2 * q.w;
		float wz = z2 * q.w;
		float xx = x2 * q.x;
		float xy = y2 * q.x;
		float xz = z2 * q.x;
		float yy = y2 * q.y;
		float yz = z2 * q.y;
		float zz = z2 * q.z;

		mat._11 = 1 - (yy + zz);
		mat._21 = xy - wz;
		mat._31 = xz + wy;
		mat._12 = xy + wz;
		mat._22 = 1 - (xx + zz);
		mat._32 = yz - wx;
		mat._13 = xz - wy;
		mat._23 = yz + wx;
		mat._33 = 1 - (xx + yy);
	}

	nCoreAPI void	quat_to_matrix(const quat& q, matrix44& mat)
	{
		float x2 = q.x * 2;
		float y2 = q.y * 2;
		float z2 = q.z * 2;
		float wx = x2 * q.w;
		float wy = y2 * q.w;
		float wz = z2 * q.w;
		float xx = x2 * q.x;
		float xy = y2 * q.x;
		float xz = z2 * q.x;
		float yy = y2 * q.y;
		float yz = z2 * q.y;
		float zz = z2 * q.z;

		mat._11 = 1 - (yy + zz);
		mat._21 = xy - wz;
		mat._31 = xz + wy;
		mat._12 = xy + wz;
		mat._22 = 1 - (xx + zz);
		mat._32 = yz - wx;
		mat._13 = xz - wy;
		mat._23 = yz + wx;
		mat._33 = 1 - (xx + yy);
	}
}//namespace nexus