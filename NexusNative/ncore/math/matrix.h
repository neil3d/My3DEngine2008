/**
*	nexus core - math
*
*	Author: Neil
*	Date:	Jan, 2008
*/

#ifndef _NEXUS_MATRIX_H_
#define _NEXUS_MATRIX_H_
#include "../base_define.h"
#include "vector.h"

namespace nexus
{
	/** matrix 3x3 for rotation */
	struct nCoreAPI matrix33
	{
		float        _11, _12, _13;
		float        _21, _22, _23;
		float        _31, _32, _33;		

		matrix33(void)	
		{
			memset(this, 0, sizeof(*this)); 
		}
		matrix33(float f11, float f12, float f13,
			float f21, float f22, float f23,
			float f31, float f32, float f33):
			_11(f11), _12(f12), _13(f13),
			_21(f21), _22(f22), _23(f23),
			_31(f31), _32(f32), _33(f33)
		{}
	};

	/** matrix 4x4 */
	struct nCoreAPI matrix44
	{	
		float        _11, _12, _13, _14;
		float        _21, _22, _23, _24;
		float        _31, _32, _33, _34;
		float        _41, _42, _43, _44;

		matrix44(void)	
		{
			memset(this, 0, sizeof(*this)); 
		}
		matrix44(float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44):
			_11(f11), _12(f12), _13(f13), _14(f14),
			_21(f21), _22(f22), _23(f23), _24(f24),
			_31(f31), _32(f32), _33(f33), _34(f34),
			_41(f41), _42(f42), _43(f43), _44(f44)
		{}

			vector3 get_axis_x() const
			{
				return vector3(_11, _21, _31);
			}
			vector3 get_axis_y() const
			{
				return vector3(_12, _22, _32);
			}
			vector3 get_axis_z() const
			{
				return vector3(_13, _23, _33);
			}
			vector3 get_origin() const
			{
				return vector3(_41, _42, _43);
			}
	};

	/** 将一个矩阵设为单位矩阵 */
	nCoreAPI void mat_set_identity(matrix44& mat);

	/** 矩阵相加 */
	nCoreAPI matrix44 operator + (const matrix44& lv, const matrix44& rv);

	/** 矩阵相减 */
	nCoreAPI matrix44 operator - (const matrix44& lv, const matrix44& rv);

	/** 矩阵乘以系数 */
	nCoreAPI matrix44 operator * (const matrix44& lv, float s);

	/** 将矩阵设置为一个左手系Look At矩阵 */
	nCoreAPI void mat_set_lookat_LH(matrix44& mat, const vector3& eye, const vector3& at, const vector3& up);

	/** 将矩阵设置为一个左手系透视矩阵*/
	nCoreAPI void mat_set_perspective_LH(matrix44& mat, float FOV, float aspect, float zNear, float zFar);

	nCoreAPI void mat_set_ortho_LH(matrix44& mat, float w, float h, float zNear, float zFar);
	
	/** 将矩阵设置为一个平移矩阵*/	
	nCoreAPI void mat_set_translation(matrix44& mat, float tx, float ty, float tz);
	inline void mat_set_translation(matrix44& mat, const vector3& offset)
	{
		mat_set_translation(mat, offset.x, offset.y, offset.z);
	}

	/** 将矩阵设置为一个缩放矩阵*/
	nCoreAPI void mat_set_scaling(matrix44& mat, float sx, float sy, float sz);

	/** 将一个矩阵设置为绕X轴旋转的矩阵*/
	nCoreAPI void mat_set_rotation_X(matrix44& mat, float angle);

	/** 将一个矩阵设置为绕Y轴旋转的矩阵*/
	nCoreAPI void mat_set_rotation_Y(matrix44& mat, float angle);

	/** 将一个矩阵设置为绕Z轴旋转的矩阵*/
	nCoreAPI void mat_set_rotation_Z(matrix44& mat, float angle);

	/** 将一个矩阵设置为旋转矩阵*/
	nCoreAPI void mat_set_rotation_yaw_pitch_roll(matrix44& mat, float yaw, float pitch, float roll);

	/** 绕指定的【单位向量】为轴，旋转一定角度*/
	nCoreAPI void mat_set_rotation_axis(matrix44& mat, float angle, const vector3& axis);

	nCoreAPI void mat_set_rotation_axis(matrix44& mat, const vector3& u, const vector3& v);

	/** 两个矩阵相乘*/
	nCoreAPI matrix44 operator * (const matrix44& lv, const matrix44& rv);

	/** 为一个矩阵添加平移操作*/	
	nCoreAPI void mat_translate(matrix44& mat, float tx, float ty, float tz);
	inline void mat_translate(matrix44& mat, const vector3& t)
	{
		mat_translate(mat, t.x, t.y, t.z);
	}

	/** 为一个矩阵添加缩放操作*/
	nCoreAPI void mat_scale(matrix44& mat, float sx, float sy, float sz);

	/** 为一个矩阵添加X轴旋转操作*/
	nCoreAPI void mat_rotate_X(matrix44& mat, float angle);

	/** 为一个矩阵添加Y轴旋转操作*/
	nCoreAPI void mat_rotate_Y(matrix44& mat, float angle);

	/** 为一个矩阵添加Z轴旋转操作*/
	nCoreAPI void mat_rotate_Z(matrix44& mat, float angle);

	/** 矩阵转置 */
	nCoreAPI matrix44 mat_transpose(const matrix44& mat);
	/** 矩阵求逆 */
	nCoreAPI matrix44 mat_inverse(const matrix44& mat);

	//-- transform vector
	nCoreAPI vector4 mat_transform(const vector4& v, const matrix44& mat);

	// 行向量
	inline vector3 operator * (const vector3& v, const matrix44& mat)
	{
		return vector3(v.x*mat._11+v.y*mat._21+v.z*mat._31+mat._41,
			v.x*mat._12+v.y*mat._22+v.z*mat._32+mat._42,
			v.x*mat._13+v.y*mat._23+v.z*mat._33+mat._43);
	}

	//-- transform normal
	/*inline vector3 mat_transform_normal(const vector3& normal, const matrix44& mat)
	{
		matrix44 matIT = mat_inverse(mat);
		matIT = mat_transpose(matIT);

		return normal*matIT;
	}*/

	template<> struct IS_POD<matrix33>
	{ 
		static const bool value = true; 
	};

	template<> struct IS_POD<matrix44>
	{ 
		static const bool value = true; 
	};
}//namespace nexus

#endif //_NEXUS_MATRIX_H_