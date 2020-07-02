#include "stdafx.h"
#include "quat.h"
#include "math_const.h"

namespace nexus
{
	const quat quat::zero(0.0f, 0.0f, 0.0f, 0.0f);
	const quat quat::identity(0.0f, 0.0f, 0.0f, 1.0f);

	quat quat::operator+ (const quat& rq) const
	{
		return quat(w+rq.w,x+rq.x,y+rq.y,z+rq.z);
	}
	quat quat::operator- (const quat& rq) const
	{
		return quat(w-rq.w,x-rq.x,y-rq.y,z-rq.z);
	}
	quat quat::operator* (const quat& rq) const
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return quat
			(
			w * rq.w - x * rq.x - y * rq.y - z * rq.z,
			w * rq.x + x * rq.w + y * rq.z - z * rq.y,
			w * rq.y + y * rq.w + z * rq.x - x * rq.z,
			w * rq.z + z * rq.w + x * rq.y - y * rq.x
			);
	}
	quat quat::operator* (float scalar) const
	{
		return quat(scalar*w,scalar*x,scalar*y,scalar*z);
	}
	quat operator* (float scalar, const quat& rq)
	{
		return quat(scalar*rq.w,scalar*rq.x,scalar*rq.y,
			scalar*rq.z);
	}
	quat quat::operator /(const quat &q) const	
	{
		quat p = quat_inverse(q); 
		return *this * p;
	}
	quat quat::operator /(float scale) const
	{ 
		return quat(x/scale,y/scale,z/scale,w/scale);
	}
	quat quat::operator- () const
	{
		return quat(-w,-x,-y,-z);
	}


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

	nCoreAPI void	quat_set_rotation(quat& q, const vector3& axis, float angle)
	{
		//// axis must be normalized
		vec_normalize(axis);
		float s = sinf(angle * 0.5f);
		q.x = s * axis.x;
		q.y = s * axis.y;
		q.z = s * axis.z;
		q.w = cosf(angle * 0.5f);		
	}

	nCoreAPI void	quat_to_matrix33(const quat& q, matrix33& mat)
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

	nCoreAPI void	quat_to_matrix44(const quat& q, matrix44& mat)
	{
#if 0
		matrix33 mat33;
		quat_to_matrix33(q, mat33);
		mat =  matrix44(mat33);
#else
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
#endif
	}

	nCoreAPI quat quat_from_rotator(const vector3& rot)
	{
		matrix44 mat;
		mat_set_rotation_yaw_pitch_roll(mat, rot.y, rot.x, rot.z);
		return matrix44_to_quat( mat );
	}

	nCoreAPI quat matrix44_to_quat(const matrix44& mat)
	{
		const float t = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + float(1);
		quat q;

		if ( t > 0 ) {
			const float s = float(0.5) / sqrt(t);
			q[3] = float(0.25) * inv(s);
			q[0] = (mat.m[2][1] - mat.m[1][2]) * s;
			q[1] = (mat.m[0][2] - mat.m[2][0]) * s;
			q[2] = (mat.m[1][0] - mat.m[0][1]) * s;
		} else {
			if ( mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2] ) {
				const float s = float(2) * sqrt( float(1) + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]);
				const float invs = inv(s);
				q[0] = float(0.25) * s;
				q[1] = (mat.m[0][1] + mat.m[1][0] ) * invs;
				q[2] = (mat.m[0][2] + mat.m[2][0] ) * invs;
				q[3] = (mat.m[1][2] - mat.m[2][1] ) * invs;
			} else if (mat.m[1][1] > mat.m[2][2]) {
				const float s = float(2) * sqrt( float(1) + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]);
				const float invs = inv(s);
				q[0] = (mat.m[0][1] + mat.m[1][0] ) * invs;
				q[1] = float(0.25) * s;
				q[2] = (mat.m[1][2] + mat.m[2][1] ) * invs;
				q[3] = (mat.m[0][2] - mat.m[2][0] ) * invs;
			} else {
				const float s = float(2) * sqrt( float(1) + mat.m[2][2] - mat.m[0][0] - mat.m[1][1] );
				const float invs = inv(s);
				q[0] = (mat.m[0][2] + mat.m[2][0] ) * invs;
				q[1] = (mat.m[1][2] + mat.m[2][1] ) * invs;
				q[2] = float(0.25) * s;
				q[3] = (mat.m[0][1] - mat.m[1][0] ) * invs;
			}
		}

		return q;
	}

	nCoreAPI quat matrix33_to_quat(const matrix33& m)
	{
		return matrix44_to_quat(matrix44(m));
	}


	// the angle is in radians
	nCoreAPI quat quat_from_axis_angle(const vector3& axis, const float a)
	{
		quat r;
		const float inv2 = 0.5f;
		const vector3 v = vec_normalize(axis) * sin(a * inv2);
		
		return quat(v.x,v.y,v.z,cos(a * inv2));
	}

	// the angle is in radians
	nCoreAPI quat quat_from_axis_angle(const float x, const float y, const float z, const float angle)
	{
		return quat_from_axis_angle(vector3(x, y, z), angle);
	}

	// the angle is stored in radians
	nCoreAPI void quat_to_axis_angle(const quat& qq, vector3* axis, float *angle)
	{
		quat q = quat_normalize(qq);

		*angle = 2 * acos(q.w);

		const float s = sin((*angle) * inv(float(2)));
		if ( s != float(0) )
			*axis = vector3(q.x, q.y, q.z) * inv(s);
		else 
			* axis = vector3(float(0), float(0), float(0));
	}

	// Spherical linear interpolation
	nCoreAPI quat quat_slerp(const quat& qq1, const quat& qq2, const float t) 
	{
		// slerp(q1,q2) = sin((1-t)*a)/sin(a) * q1  +  sin(t*a)/sin(a) * q2
		const quat q1 = quat_normalize(qq1);
		const quat q2 = quat_normalize(qq2);

		const float a = acos(quat_dot(q1, q2));
		const float s = sin(a);

		if ( !(-nEPS <= s && s <= nEPS) ) {
			return sin((float(1)-t)*a)/s * q1  +  sin(t*a)/s * q2;
		} else {
			// if the angle is to small use a linear interpolation
			return lerp(q1, q2, t);
		}
	}

	// Sperical quadtratic interpolation using a smooth cubic spline
	// The parameters a and b are the control points.
	nCoreAPI quat quat_squad(
		const quat& q0, 
		const quat& a, 
		const quat& b, 
		const quat& q1, 
		const float t) 
	{
		return quat_slerp(quat_slerp(q0, q1, t),quat_slerp(a, b, t), 2 * t * (1 - t));
	}

}//namespace nexus