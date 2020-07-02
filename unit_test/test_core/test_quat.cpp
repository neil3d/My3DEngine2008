#include "ncore.h"
#include <boost/test/unit_test.hpp>
#include <d3dx10math.h>
#include <iostream>
using namespace std;

#include "nv_math/nv_math.h"

//const static float TOLERANCE = MathConst::DELTA;
const static float TOLERANCE = 0;

bool quat_equal(const nexus::quat& fq, const D3DXQUATERNION& dq, float t = 0)
{
	bool bEqual = true;

	if(fabsf(fq.x - dq.x) > t)
	{
		bEqual = false;
		cout << "quat error: x, difference = "
			<< fq.x - dq.x << endl;
	}

	if(fabsf(fq.y - dq.y) > t)
	{
		bEqual = false;
		cout << "quat error: x, difference = "
			<< fq.y - dq.y << endl;
	}

	if(fabsf(fq.z - dq.z) > t)
	{
		bEqual = false;
		cout << "quat error: x, difference = "
			<< fq.z - dq.z << endl;
	}

	if(fabsf(fq.w - dq.w) > t)
	{
		bEqual = false;
		cout << "quat error: x, difference = "
			<< fq.w - dq.w << endl;
	}

	return bEqual;
}

BOOST_AUTO_TEST_CASE(test_quat_identity)
{
	nexus::quat fq;
	quat_set_identity(fq);

	D3DXQUATERNION dq;
	D3DXQuaternionIdentity(&dq);

	BOOST_CHECK(quat_equal(fq,dq,0));
}

float my_rand()
{
	return (rand()%100)/64.0f;
}

bool quat_equal(const nexus::quat& fq, const quat& nv_q)
{
	return fq.x==nv_q.x
		&&  fq.y==nv_q.y
		&&  fq.z==nv_q.z
		&&  fq.w==nv_q.w;
}

// 测试一些简单函数
BOOST_AUTO_TEST_CASE(test_quat_simple)
{
	nexus::quat fq(my_rand(), my_rand(), my_rand(), my_rand());
	quat nvq(fq.x, fq.y, fq.z, fq.w);
	
	//-- 测试取反
	nexus::quat fq_inv = quat_inverse(fq);
	quat nvq_inv = nvq.Inverse();

	BOOST_CHECK(quat_equal(fq_inv, nvq_inv));

	//-- 测试规一化
	nexus::quat fq_norm = quat_normalize(fq);
	quat nvq_normal = nvq;
	nvq_normal.Normalize();

	BOOST_CHECK(quat_equal(fq_norm, nvq_normal));

	//-- 测试点积
	nexus::quat fq2(my_rand(), my_rand(), my_rand(), my_rand());
	quat nvq2(fq2.x, fq2.y, fq2.z, fq2.w);

	BOOST_CHECK_EQUAL(nexus::quat_dot(fq, fq2),
		dot(nvq, nvq2));

	//-- 测试球面差值
	nexus::quat fq_lerp;
	quat nvq_lerp;

	float r = (rand()%100)/100.0f;
	
	fq_lerp = nexus::quat_slerp(fq, fq2, r);
	slerp_quats(nvq_lerp, r, nvq, nvq2);

	BOOST_CHECK(quat_equal(fq_lerp, nvq_lerp));

}

bool mat3_equat(const nexus::matrix33& m3, const mat3& nv_m3)
{
	const float* p1 = &m3._11;
	const float* p2 = &nv_m3.a00;

	for(int i=0; i<9; i++)
	{
		if(p1[i] != p2[i])
			return false;
	}
	return true;
}
bool mat4_equat(const nexus::matrix44& m4, const mat4& nv_m4)
{
	const float* p1 = &m4._11;
	const float* p2 = &nv_m4.a00;

	for(int i=0; i<16; i++)
	{
		if(p1[i] != p2[i])
			return false;
	}
	return true;
}

BOOST_AUTO_TEST_CASE(test_quat_rot)
{
	// axis-angle
	nexus::vector3 axis(my_rand(), my_rand(), my_rand());
	axis = vec_normalize(axis);
	float angle = my_rand();

	nexus::quat fq;
	quat nv_q;

	nexus::quat_set_rotation(fq, axis, angle);
	axis_to_quat(nv_q, vec3(&axis.x), angle);

	BOOST_CHECK(quat_equal(fq, nv_q));

	//-- to matrix3
	nexus::matrix33 m3;
	quat_to_matrix(fq, m3);

	mat3 nv_mat3;
	quat_2_mat(nv_mat3, nv_q);

	BOOST_CHECK(mat3_equat(m3, nv_mat3));

	//-- to mat4
	nexus::matrix44 m4;
	quat_to_matrix(fq, m4);

	mat4 nv_mat4 = mat4_null;
	nv_mat4.set_rot(nv_mat3);

	BOOST_CHECK(mat4_equat(m4, nv_mat4));
}