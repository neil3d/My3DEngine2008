#include "stdafx.h"
#include "matrix.h"

namespace nexus
{
	vector3 vector3::zero(0,0,0);
	vector3 vector3::one(1,1,1);
	
	const matrix44 matrix44::identity
		(
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1
		);

	nCoreAPI void mat_set_identity(matrix44& mat)
	{
		memset(&mat, 0, sizeof(matrix44));

		mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	}

	nCoreAPI matrix44 operator + (const matrix44& lv, const matrix44& rv)
	{
		return matrix44(lv._11+rv._11, lv._12+rv._12, lv._13+rv._13, lv._14+rv._14,
			lv._21+rv._21, lv._22+rv._22, lv._23+rv._23, lv._24+rv._24,
			lv._31+rv._31, lv._32+rv._32, lv._33+rv._33, lv._34+rv._34,
			lv._41+rv._41, lv._42+rv._42, lv._43+rv._43, lv._44+rv._44);
	}

	nCoreAPI matrix44 operator - (const matrix44& lv, const matrix44& rv)
	{
		return matrix44(lv._11-rv._11, lv._12-rv._12, lv._13-rv._13, lv._14-rv._14,
			lv._21-rv._21, lv._22-rv._22, lv._23-rv._23, lv._24-rv._24,
			lv._31-rv._31, lv._32-rv._32, lv._33-rv._33, lv._34-rv._34,
			lv._41-rv._41, lv._42-rv._42, lv._43-rv._43, lv._44-rv._44);
	}

	nCoreAPI matrix44 operator * (const matrix44& lv, float s)
	{
		return matrix44(s*lv._11, s*lv._12, s*lv._13, s*lv._14,
			s*lv._21, s*lv._22, s*lv._23, s*lv._24,
			s*lv._31, s*lv._32, s*lv._33, s*lv._34,
			s*lv._41, s*lv._42, s*lv._43, s*lv._44);
	}

	nCoreAPI void mat_set_lookat_LH(matrix44& mat, const vector3& eye, const vector3& at, const vector3& up)
	{
		vector3 zAxis = vec_normalize(at - eye);
		vector3 xAxis = vec_normalize(vec_cross(up, zAxis));
		vector3 yAxis = vec_cross(zAxis, xAxis);

		mat._11 = xAxis.x;	mat._12 = yAxis.x;	mat._13 = zAxis.x;	mat._14 = 0;
		mat._21 = xAxis.y;	mat._22 = yAxis.y;	mat._23 = zAxis.y;	mat._24 = 0;
		mat._31 = xAxis.z;	mat._32 = yAxis.z;	mat._33 = zAxis.z;	mat._34 = 0;

		mat._41 = -vec_dot(xAxis, eye);	
		mat._42 = -vec_dot(yAxis, eye);
		mat._43 = -vec_dot(zAxis, eye);
		mat._44 = 1;
	}


	nCoreAPI void mat_set_perspective_LH(matrix44& mat, float FOV, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f/tanf(FOV*0.5f);
		float xScale = yScale/aspect;

		mat._11 = xScale;	mat._12 = 0;		mat._13 = 0;					mat._14 = 0;
		mat._21 = 0;		mat._22 = yScale;	mat._23 = 0;					mat._24 = 0;
		mat._31 = 0;		mat._32 = 0;		mat._33 = zFar/(zFar-zNear);	mat._34 = 1;
		mat._41 = 0;		mat._42 = 0;		mat._43 = -zNear*zFar/(zFar-zNear);	mat._44 = 0;		
	}

	nCoreAPI void mat_set_ortho_LH(matrix44& mat, float w, float h, float zn, float zf)
	{
		mat._11 = 2/w;	mat._12 = 0;	mat._13 = 0;	mat._14 = 0;
		mat._21 = 0;	mat._22 = 2/h;	mat._23 = 0;	mat._24 = 0;
		mat._31 = 0;	mat._32 = 0;	mat._33 = 1/(zf-zn); mat._34 = 0;
		mat._41 = 0;	mat._42 = 0;	mat._43 = -zn/(zf-zn);	mat._44 = 1;
	}

	nCoreAPI void mat_set_translation(matrix44& mat, float tx, float ty, float tz)
	{
		mat._11 = 1;	mat._12 = 0;	mat._13 = 0;	mat._14 = 0;
		mat._21 = 0;	mat._22 = 1;	mat._23 = 0;	mat._24 = 0;
		mat._31 = 0;	mat._32 = 0;	mat._33 = 1;	mat._34 = 0;
		mat._41 = tx;	mat._42 = ty;	mat._43 = tz;	mat._44 = 1;
	}

	nCoreAPI void mat_set_scaling(matrix44& mat, float sx, float sy, float sz)
	{
		mat._11 = sx;	mat._12 = 0;	mat._13 = 0;	mat._14 = 0;
		mat._21 = 0;	mat._22 = sy;	mat._23 = 0;	mat._24 = 0;
		mat._31 = 0;	mat._32 = 0;	mat._33 = sz;	mat._34 = 0;
		mat._41 = 0;	mat._42 = 0;	mat._43 = 0;	mat._44 = 1;
	}

	nCoreAPI void mat_set_rotation_X(matrix44& mat, float angle)
	{
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		mat._11 = 1;	mat._12 = 0;	mat._13 = 0;	mat._14 = 0;
		mat._21 = 0;	mat._22 = cosA;	mat._23 = sinA;	mat._24 = 0;
		mat._31 = 0;	mat._32 = -sinA;mat._33 = cosA;	mat._34 = 0;
		mat._41 = 0;	mat._42 = 0;	mat._43 = 0;	mat._44 = 1;
	}

	nCoreAPI void mat_set_rotation_Y(matrix44& mat, float angle)
	{
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		mat._11 = cosA;	mat._12 = 0;	mat._13 =-sinA;	mat._14 = 0;
		mat._21 = 0;	mat._22 = 1;	mat._23 = 0;	mat._24 = 0;
		mat._31 = sinA;	mat._32 = 0;	mat._33 = cosA;	mat._34 = 0;
		mat._41 = 0;	mat._42 = 0;	mat._43 = 0;	mat._44 = 1;
	}

	nCoreAPI void mat_set_rotation_Z(matrix44& mat, float angle)
	{
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		mat._11 = cosA;	mat._12 = sinA;	mat._13 = 0;	mat._14 = 0;
		mat._21 =-sinA;	mat._22 = cosA;	mat._23 = 0;	mat._24 = 0;
		mat._31 = 0;	mat._32 = 0;	mat._33 = 1;	mat._34 = 0;
		mat._41 = 0;	mat._42 = 0;	mat._43 = 0;	mat._44 = 1;
	}

	nCoreAPI matrix44 operator * (const matrix44& m1, const matrix44& m2)
	{
		union 
		{ 
			__m128 r[4];
			_matrix44 matrix;
		}  lv,rv,result;
		
		lv.matrix = *(_matrix44*)&m1;
		rv.matrix = *(_matrix44*)&m2;

		// Use vW to hold the original row
		__m128 vW = lv.r[0];
		// Splat the component X,Y,Z then W
		__m128 vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		__m128 vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		__m128 vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		// Perform the opertion on the first row
		vX = _mm_mul_ps(vX,rv.r[0]);
		vY = _mm_mul_ps(vY,rv.r[1]);
		vZ = _mm_mul_ps(vZ,rv.r[2]);
		vW = _mm_mul_ps(vW,rv.r[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX,vZ);
		vY = _mm_add_ps(vY,vW);
		vX = _mm_add_ps(vX,vY);
		result.r[0] = vX;
		// Repeat for the other 3 rows
		vW = lv.r[1];
		vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		vX = _mm_mul_ps(vX,rv.r[0]);
		vY = _mm_mul_ps(vY,rv.r[1]);
		vZ = _mm_mul_ps(vZ,rv.r[2]);
		vW = _mm_mul_ps(vW,rv.r[3]);
		vX = _mm_add_ps(vX,vZ);
		vY = _mm_add_ps(vY,vW);
		vX = _mm_add_ps(vX,vY);
		result.r[1] = vX;
		vW = lv.r[2];
		vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		vX = _mm_mul_ps(vX,rv.r[0]);
		vY = _mm_mul_ps(vY,rv.r[1]);
		vZ = _mm_mul_ps(vZ,rv.r[2]);
		vW = _mm_mul_ps(vW,rv.r[3]);
		vX = _mm_add_ps(vX,vZ);
		vY = _mm_add_ps(vY,vW);
		vX = _mm_add_ps(vX,vY);
		result.r[2] = vX;
		vW = lv.r[3];
		vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		vX = _mm_mul_ps(vX,rv.r[0]);
		vY = _mm_mul_ps(vY,rv.r[1]);
		vZ = _mm_mul_ps(vZ,rv.r[2]);
		vW = _mm_mul_ps(vW,rv.r[3]);
		vX = _mm_add_ps(vX,vZ);
		vY = _mm_add_ps(vY,vW);
		vX = _mm_add_ps(vX,vY);
		result.r[3] = vX;
		return *(matrix44*)&result.matrix;
	}

	nCoreAPI void mat_translate(matrix44& mat, float tx, float ty, float tz)
	{
		// mat = mat*matTranslation
		mat._11 = mat._11+mat._14*tx;	mat._12 = mat._12+mat._14*ty;	mat._13 = mat._13+mat._14*tz;	// mat._14 no change
		mat._21 = mat._21+mat._24*tx;	mat._22 = mat._22+mat._24*ty;	mat._23 = mat._23+mat._24*tz;	// mat._24 no change
		mat._31 = mat._31+mat._34*tx;	mat._32 = mat._32+mat._34*ty;	mat._33 = mat._33+mat._34*tz;	// mat._34 no change
		mat._41 = mat._41+mat._44*tx;	mat._42 = mat._42+mat._44*ty;	mat._43 = mat._43+mat._44*tz;	// mat._44 no change
	}

	nCoreAPI void mat_scale(matrix44& mat, float sx, float sy, float sz)
	{
		// mat = mat*mat_scale
		mat._11 = mat._11*sx;	mat._12 = mat._12*sy;	mat._13 = mat._13*sz;	//mat._14 no change
		mat._21 = mat._21*sx;	mat._22 = mat._22*sy;	mat._23 = mat._23*sz;	//mat._24 no change
		mat._31 = mat._31*sx;	mat._32 = mat._32*sy;	mat._33 = mat._33*sz;	//mat._34 no change
		mat._41 = mat._41*sx;	mat._42 = mat._42*sy;	mat._43 = mat._43*sz;	//mat._44 no change
	}

	nCoreAPI void mat_rotate_X(matrix44& mat, float angle)
	{
		// mat = mat*matRotate
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		matrix44 matT(mat);
		/*mat_11*/	mat._12 = matT._12*cosA-matT._13*sinA;	mat._13 = matT._12*sinA+matT._13*cosA;	/*mat._14*/
		/*mat_21*/	mat._22 = matT._22*cosA-matT._23*sinA;	mat._23 = matT._22*sinA+matT._23*cosA;	/*mat._24*/
		/*mat_31*/	mat._32 = matT._32*cosA-matT._33*sinA;	mat._33 = matT._32*sinA+matT._33*cosA;	/*mat._34*/
		/*mat_41*/	mat._42 = matT._42*cosA-matT._43*sinA;	mat._43 = matT._42*sinA+matT._43*cosA;	/*mat._44*/
	}

	nCoreAPI void mat_rotate_Y(matrix44& mat, float angle)
	{
		// mat = mat*matRotate
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		matrix44 matT(mat);

		mat._11 = matT._11*cosA+matT._13*sinA;	/*mat._12*/	mat._13 = -matT._11*sinA+matT._13*cosA;	/*mat._14*/
		mat._21 = matT._21*cosA+matT._23*sinA;	/*mat._22*/	mat._23 = -matT._21*sinA+matT._23*cosA;	/*mat._24*/
		mat._31 = matT._31*cosA+matT._33*sinA;	/*mat._32*/	mat._33 = -matT._31*sinA+matT._33*cosA;	/*mat._34*/
		mat._41 = matT._41*cosA+matT._43*sinA;	/*mat._42*/	mat._43 = -matT._41*sinA+matT._43*cosA;	/*mat._44*/
	}

	nCoreAPI void mat_rotate_Z(matrix44& mat, float angle)
	{
		// mat = mat*matRotate
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		matrix44 matT(mat);

		mat._11 = matT._11*cosA-matT._12*sinA;	mat._12 = matT._11*sinA+mat._12*cosA;	/*mat._13, mat._14*/
		mat._21 = matT._21*cosA-matT._22*sinA;	mat._22 = matT._21*sinA+mat._22*cosA;	/*mat._23, mat._24*/
		mat._31 = matT._31*cosA-matT._32*sinA;	mat._32 = matT._31*sinA+mat._32*cosA;	/*mat._33, mat._34*/
		mat._41 = matT._41*cosA-matT._42*sinA;	mat._42 = matT._41*sinA+mat._42*cosA;	/*mat._43, mat._44*/
	}

	nCoreAPI void mat_set_rotation_yaw_pitch_roll(matrix44& mat, float yaw, float pitch, float roll)
	{
		// D3D doc : The order of transformations is roll first, then pitch, then yaw. 
		mat_set_identity(mat);
		mat_rotate_Z(mat, roll);
		mat_rotate_X(mat, pitch);
		mat_rotate_Y(mat, yaw);		
	}

	nCoreAPI matrix44 mat_transpose(const matrix44& mat)
	{
		return matrix44(
			mat._11, mat._21, mat._31, mat._41,
			mat._12, mat._22, mat._32, mat._42,
			mat._13, mat._23, mat._33, mat._43,
			mat._14, mat._24, mat._34, mat._44
			);
	}

	/*
	calculate the determinent of a 2x2 matrix in the from

	| a1 a2 |
	| b1 b2 |

	*/
	float det2x2(float a1, float a2, float b1, float b2)
	{
		return a1 * b2 - b1 * a2;
	}

	/*
	calculate the determinent of a 3x3 matrix in the from

	| a1 a2 a3 |
	| b1 b2 b3 |
	| c1 c2 c3 |

	*/
	float det3x3(float a1, float a2, float a3, 
		float b1, float b2, float b3, 
		float c1, float c2, float c3)
	{
		return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
	}

	nCoreAPI matrix44 mat_inverse(const matrix44& mat)
	{
		const matrix44& A = mat;	// for short
		matrix44 B;
		
		float det,oodet;

		B._11 =  det3x3(A._22, A._23, A._24, A._32, A._33, A._34, A._42, A._43, A._44);
		B._12 = -det3x3(A._12, A._13, A._14, A._32, A._33, A._34, A._42, A._43, A._44);
		B._13 =  det3x3(A._12, A._13, A._14, A._22, A._23, A._24, A._42, A._43, A._44);
		B._14 = -det3x3(A._12, A._13, A._14, A._22, A._23, A._24, A._32, A._33, A._34);

		B._21 = -det3x3(A._21, A._23, A._24, A._31, A._33, A._34, A._41, A._43, A._44);
		B._22 =  det3x3(A._11, A._13, A._14, A._31, A._33, A._34, A._41, A._43, A._44);
		B._23 = -det3x3(A._11, A._13, A._14, A._21, A._23, A._24, A._41, A._43, A._44);
		B._24 =  det3x3(A._11, A._13, A._14, A._21, A._23, A._24, A._31, A._33, A._34);

		B._31 =  det3x3(A._21, A._22, A._24, A._31, A._32, A._34, A._41, A._42, A._44);
		B._32 = -det3x3(A._11, A._12, A._14, A._31, A._32, A._34, A._41, A._42, A._44);
		B._33 =  det3x3(A._11, A._12, A._14, A._21, A._22, A._24, A._41, A._42, A._44);
		B._34 = -det3x3(A._11, A._12, A._14, A._21, A._22, A._24, A._31, A._32, A._34);

		B._41 = -det3x3(A._21, A._22, A._23, A._31, A._32, A._33, A._41, A._42, A._43);
		B._42 =  det3x3(A._11, A._12, A._13, A._31, A._32, A._33, A._41, A._42, A._43);
		B._43 = -det3x3(A._11, A._12, A._13, A._21, A._22, A._23, A._41, A._42, A._43);
		B._44 =  det3x3(A._11, A._12, A._13, A._21, A._22, A._23, A._31, A._32, A._33);

		det = (A._11 * B._11) + (A._21 * B._12) + (A._31 * B._13) + (A._41 * B._14);

		// The following divions goes unchecked for division
		// by zero. We should consider throwing an exception
		// if det < eps.
		oodet = 1.0f / det;

		B._11 *= oodet;
		B._12 *= oodet;
		B._13 *= oodet;
		B._14 *= oodet;

		B._21 *= oodet;
		B._22 *= oodet;
		B._23 *= oodet;
		B._24 *= oodet;

		B._31 *= oodet;
		B._32 *= oodet;
		B._33 *= oodet;
		B._34 *= oodet;

		B._41 *= oodet;
		B._42 *= oodet;
		B._43 *= oodet;
		B._44 *= oodet;

		return B;
	}

	nCoreAPI void mat_set_rotation_axis(matrix44& mat, float angle, const vector3& axis)
	{
		const vector3& v = axis;
		float ct = cosf(angle);
		float st = sinf(angle);

		float xx = v.x * v.x;
		float yy = v.y * v.y;
		float zz = v.z * v.z;
		float xy = v.x * v.y;
		float xz = v.x * v.z;
		float yz = v.y * v.z;

		mat._11 = xx + ct*(1-xx);
		mat._21 = xy + ct*(-xy) + st*-v.z;
		mat._31 = xz + ct*(-xz) + st*v.y;

		mat._12 = xy + ct*(-xy) + st*v.z;
		mat._22 = yy + ct*(1-yy);
		mat._32 = yz + ct*(-yz) + st*-v.x;

		mat._13 = xz + ct*(-xz) + st*-v.y;
		mat._23 = yz + ct*(-yz) + st*v.x;
		mat._33 = zz + ct*(1-zz);
	}

	nCoreAPI void mat_set_rotation_axis(matrix44& mat, const vector3& u, const vector3& v)
	{
		mat_set_identity(mat);

		float phi;
		float h;
		float lambda;
		
		vector3 w = vec_cross(u,v);

		phi = vec_dot(u,v);
		lambda = vec_dot(w,w);

		if (lambda > nEPS)
			h = (1.0f - phi) / lambda;
		else
			h = lambda;

		float hxy = w.x * w.y * h;
		float hxz = w.x * w.z * h;
		float hyz = w.y * w.z * h;

		mat._11 = phi + w.x * w.x * h;
		mat._21 = hxy - w.z;
		mat._31 = hxz + w.y;

		mat._12 = hxy + w.z;
		mat._22 = phi + w.y * w.y * h;
		mat._32 = hyz - w.x;

		mat._13 = hxz - w.y;
		mat._23 = hyz + w.x;
		mat._33 = phi + w.z * w.z * h;
	}

	vector4 mat_transform(const vector4& v, const matrix44& mat)
	{
		return vector4(v.x*mat._11+v.y*mat._21+v.z*mat._31+v.w*mat._41,
			v.x*mat._12+v.y*mat._22+v.z*mat._32+v.w*mat._42,
			v.x*mat._13+v.y*mat._23+v.z*mat._33+v.w*mat._43,
			v.x*mat._14+v.y*mat._24+v.z*mat._34+v.w*mat._44
			);
	}
}//namespace nexus