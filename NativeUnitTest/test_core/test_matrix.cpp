#include "ncore.h"
#include "math/object_space.h"
#include <boost/test/unit_test.hpp>
#include "d3dx10math.h"
#include <iostream>
using namespace std;

//#define USE_D3D_MATH
#define USE_NV_MATH
#include "nv_math/nv_math.h"

using namespace nexus;

//const static float TOLERANCE = MathConst::DELTA;
const static float TOLERANCE = 0.00001f;

#pragma warning(disable:4244)//int to float

bool mat_equal(const matrix44& nMat, const mat4& nv_mat)
{
	const float* m = &nMat._11;
	const float* nm = &nv_mat._11;
	
	for(int i=0;i<4*4;i++)
	{
		if(m[i] != nm[i])
			return false;
	}
	return true;
}

bool mat_equal(const matrix44& nMat, const D3DXMATRIX& dMat, float t=0)
{
	const float* m = &nMat._11;
	bool ret = true;
	for(int j=0;j<4;j++)
		for(int i=0;i<4;i++)
		{
			if(fabsf(m[j*4+i]-dMat.m[j][i]) > t)
			{
				cout << "matrix error, index = _"
					<< j+1 << i+1 << ", "
					<< "difference = "
					<< m[j*4+i] - dMat.m[j][i]
				<< endl;
				ret = false;
			}
		}

		return ret;
}

// identity
BOOST_AUTO_TEST_CASE(testMatIdentity)
{
	matrix44 nMat;
	mat_set_identity(nMat);

	D3DXMATRIX dMat;
	D3DXMatrixIdentity(&dMat);

	BOOST_CHECK(mat_equal(nMat, dMat,0));
}

#define DEFINE_MAT matrix44 nMat1(rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100);\
	matrix44 nMat2(rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100,\
	rand()%100,rand()%100,rand()%100,rand()%100);\
	matrix44 nMat3;\
	D3DXMATRIX dMat1((float*)&nMat1), dMat2((float*)&nMat2), dMat3;

//BOOST_AUTO_TEST_CASE(testTransformNormal)
//{
//	vector3 v(rand()%100,rand()%100,rand()%100);
//	v = vec_normalize(v);
//	DEFINE_MAT
//
//	vector3 t = mat_transform_normal(v, nMat1);
//
//	float len = fabsf(vec_length(t));
//	BOOST_CHECK( (len-1.0f) < 0.00001f);
//
//	D3DXVECTOR3 dv;
//	D3DXVec3TransformNormal(&dv, (const D3DXVECTOR3*)&v, &dMat1);
//
//	BOOST_CHECK_EQUAL(t.x, dv.x);
//	BOOST_CHECK_EQUAL(t.y, dv.y);
//	BOOST_CHECK_EQUAL(t.z, dv.z);
//}

// transform vector3
BOOST_AUTO_TEST_CASE(tsetTransformV3)
{
	vector3 v(rand()%100,rand()%100,rand()%100);
	DEFINE_MAT

	vector3 t = v*nMat1;

	D3DXVECTOR4 dv;
	D3DXVec3Transform(&dv, (const D3DXVECTOR3*)&v, &dMat1);

	BOOST_CHECK_EQUAL(t.x, dv.x);
	BOOST_CHECK_EQUAL(t.y, dv.y);
	BOOST_CHECK_EQUAL(t.z, dv.z);
}

// inverse
BOOST_AUTO_TEST_CASE(testMatInverse)
{
	DEFINE_MAT
	nMat3 = mat_inverse(nMat1);

#ifdef USE_NV_MATH
	mat4 nv_mat(&nMat1._11);
	mat4 nv_inv;
	invert(nv_inv, nv_mat);

	BOOST_CHECK(mat_equal(nMat3, nv_inv))	;
#endif

#ifdef USE_D3D
	// check 1
	D3DXMatrixInverse(&dMat3, NULL, &dMat1);

	BOOST_CHECK(mat_equal(nMat3, dMat3, 0));

	//-- check 2
	matrix44 mat_I = nMat3*nMat1;
	D3DXMATRIX dMat_I;
	D3DXMatrixIdentity(&dMat_I);

	BOOST_CHECK(mat_equal(mat_I, dMat_I, 1e-6f));
#endif
}

// transpose
BOOST_AUTO_TEST_CASE(testMatTranspose)
{
	DEFINE_MAT

	nMat3 = mat_transpose(nMat1);
	D3DXMatrixTranspose(&dMat3, &dMat1);

	BOOST_CHECK(mat_equal(nMat3, dMat3, 0));
}

// add
BOOST_AUTO_TEST_CASE(testMatAdd)
{
	DEFINE_MAT

	nMat3 = nMat1+nMat2;
	dMat3 = dMat1+dMat2;
	
	BOOST_CHECK(mat_equal(nMat3, dMat3, 0));
}

// sub
BOOST_AUTO_TEST_CASE(testMatSub)
{
	DEFINE_MAT

	nMat3 = nMat1-nMat2;
	dMat3 = dMat1-dMat2;

	BOOST_CHECK(mat_equal(nMat3, dMat3, 0));
}

// scale
BOOST_AUTO_TEST_CASE(testMatScale)
{
	DEFINE_MAT

	float s=rand()%100;
	nMat3 = nMat1*s;
	dMat3 = dMat1*s;
	
	BOOST_CHECK(mat_equal(nMat3, dMat3, 0));
}

// look at
BOOST_AUTO_TEST_CASE(testMatLookAt)
{
	vector3 eye(100, 100, 100), at(20,20,20), up(0,1,0);
	matrix44 nMatLookAt;
	mat_set_lookat_LH(nMatLookAt, eye, at, up);

	D3DXMATRIX dMatLookAt;
	D3DXMatrixLookAtLH(&dMatLookAt, (D3DXVECTOR3*)&eye, (D3DXVECTOR3*)&at, (D3DXVECTOR3*)&up);

	BOOST_CHECK(mat_equal(nMatLookAt, dMatLookAt, 0));
}

// perspective
BOOST_AUTO_TEST_CASE(testMatPerspective)
{
	float FOV=3.14f/3.0f, aspect=640/480.0f, zNear=10, zFar=5000;
	matrix44 nMatP;
	mat_set_perspective_LH(nMatP, FOV, aspect, zNear, zFar);

	D3DXMATRIX dMatP;
	D3DXMatrixPerspectiveFovLH(&dMatP, FOV, aspect, zNear, zFar);

	BOOST_CHECK(mat_equal(nMatP, dMatP, 0));
}

// set translate
BOOST_AUTO_TEST_CASE(testMatSetTrans)
{
	vector3 trans(rand()%100, rand()%100, rand()%100);

	matrix44 nMatTrans;
	mat_set_translation(nMatTrans, trans.x, trans.y, trans.z);

	D3DXMATRIX dMatTrans;
	D3DXMatrixTranslation(&dMatTrans, trans.x, trans.y, trans.z);

	BOOST_CHECK(mat_equal(nMatTrans, dMatTrans));
}

// set scale
BOOST_AUTO_TEST_CASE(testMatSetScale)
{
	vector3 trans(rand()%100, rand()%100, rand()%100);

	matrix44 nMatScale;
	mat_set_scaling(nMatScale, trans.x, trans.y, trans.z);

	D3DXMATRIX dMatScale;
	D3DXMatrixScaling(&dMatScale, trans.x, trans.y, trans.z);

	BOOST_CHECK(mat_equal(nMatScale, dMatScale));
}

// set rotation XYZ
BOOST_AUTO_TEST_CASE(testMatSetRotXYZ)
{
	vector3 trans(rand()%100, rand()%100, rand()%100);

	matrix44 nMatR;
	D3DXMATRIX dMatR;

	// X
	mat_set_rotation_X(nMatR, trans.x);
	D3DXMatrixRotationX(&dMatR, trans.x);
	BOOST_CHECK(mat_equal(nMatR, dMatR));

	// Y
	mat_set_rotation_Y(nMatR, trans.y);
	D3DXMatrixRotationY(&dMatR, trans.y);
	BOOST_CHECK(mat_equal(nMatR, dMatR));

	// Z
	mat_set_rotation_Z(nMatR, trans.z);
	D3DXMatrixRotationZ(&dMatR, trans.z);
	BOOST_CHECK(mat_equal(nMatR, dMatR));
}

// mul
BOOST_AUTO_TEST_CASE(testMatMul)
{
	DEFINE_MAT

	matrix44 nMatMul = nMat1*nMat2;
	D3DXMATRIX dMatMul = dMat1*dMat2;

	BOOST_CHECK(mat_equal(nMatMul, dMatMul));
}

// combine
BOOST_AUTO_TEST_CASE(testMatCombine)
{
	vector3 trans(rand()%100, rand()%100, rand()%100);

	matrix44 nMatT;
	D3DXMATRIX dMatT, dMatTemp;

	// translate
	mat_set_identity(nMatT);
	mat_translate(nMatT, trans.x, trans.y, trans.z);

	D3DXMatrixTranslation(&dMatT, trans.x, trans.y, trans.z);

	BOOST_CHECK(mat_equal(nMatT, dMatT));


	// scale 
	mat_scale(nMatT, trans.x, trans.y, trans.z);

	D3DXMatrixScaling(&dMatTemp, trans.x, trans.y, trans.z);
	dMatT = dMatT*dMatTemp;
	
	BOOST_CHECK(mat_equal(nMatT, dMatT));

	// rotate x
	mat_rotate_X(nMatT, trans.x);

	D3DXMatrixRotationX(&dMatTemp, trans.x);
	dMatT = dMatT*dMatTemp;

	BOOST_CHECK(mat_equal(nMatT, dMatT, TOLERANCE));

	// rotate y
	mat_rotate_Y(nMatT, trans.y);

	D3DXMatrixRotationY(&dMatTemp, trans.y);
	dMatT = dMatT*dMatTemp;

	BOOST_CHECK(mat_equal(nMatT, dMatT, TOLERANCE));

	// rotate z
	mat_rotate_Z(nMatT, trans.z);

	D3DXMatrixRotationZ(&dMatTemp, trans.z);
	dMatT = dMatT*dMatTemp;

	BOOST_CHECK(mat_equal(nMatT, dMatT, TOLERANCE));
}

// yaw pitch roll
BOOST_AUTO_TEST_CASE(testMatYawPitchRoll)
{
	vector3 trans(rand()%100, rand()%100, rand()%100);

	matrix44 matYPR;
	mat_set_rotation_yaw_pitch_roll(matYPR, trans.y, trans.x, trans.z);

	D3DXMATRIX dMatTemp;
	D3DXMatrixRotationYawPitchRoll(&dMatTemp, trans.y, trans.x, trans.z);

	BOOST_CHECK(mat_equal(matYPR, dMatTemp, TOLERANCE));
}

// axis-angle rotate
BOOST_AUTO_TEST_CASE(testAxisAngleRot)
{
	matrix44 my_mat;
	mat_set_identity(my_mat);

	mat4 nv_mat(&my_mat._11);

	vector3 axis(rand()%100, rand()%100, rand()%100);
	axis = vec_normalize(axis);
	float r = rand()%100;

	mat_set_rotation_axis(my_mat, r, axis);
	nv_mat.set_rot(r, vec3(&axis.x));

	BOOST_CHECK(mat_equal(my_mat, nv_mat));
	
}

BOOST_AUTO_TEST_CASE(testAxis2Rot)
{
	matrix44 my_mat;
	mat_set_identity(my_mat);

	mat4 nv_mat(&my_mat._11);

	vector3 axis1(rand()%100, rand()%100, rand()%100);
	axis1 = vec_normalize(axis1);

	vector3 axis2(rand()%100, rand()%100, rand()%100);
	axis2 = vec_normalize(axis1);	

	mat_set_rotation_axis(my_mat, axis1, axis2);
	nv_mat.set_rot(vec3(&axis1.x), vec3(&axis2.x));

	BOOST_CHECK(mat_equal(my_mat, nv_mat));

}

BOOST_AUTO_TEST_CASE(testVec4Transform)
{
	vector4 v(rand()%100,rand()%100,rand()%100,rand()%100);
	DEFINE_MAT

	vector4 t = mat_transform(v, nMat1);
	

	D3DXVECTOR4 dv;
	D3DXVec4Transform(&dv, (const D3DXVECTOR4*)&v, &dMat1);

	BOOST_CHECK_EQUAL(t.x, dv.x);
	BOOST_CHECK_EQUAL(t.y, dv.y);
	BOOST_CHECK_EQUAL(t.z, dv.z);
	BOOST_CHECK_EQUAL(t.w, dv.w);
}

BOOST_AUTO_TEST_CASE(testReflectMat)
{	
	plane pln;
	pln.normal = vector3(rand()%100,rand()%100,rand()%100);
	pln.dist = rand()%100;
	pln.normalize();

	
	matrix44 ref_mat;
	mat_reflect(ref_mat, pln);

	D3DXMATRIX dMat;
	D3DXMatrixReflect(&dMat, (const D3DXPLANE*)&pln);

	BOOST_CHECK(mat_equal(ref_mat, dMat,0));
}

BOOST_AUTO_TEST_CASE(testOrtho)
{
	matrix44 mat;
	mat_set_ortho_LH(mat, 512, 512, 5, 50000);

	D3DXMATRIX dMat;
	D3DXMatrixOrthoLH(&dMat, 512, 512, 5, 50000);

	BOOST_CHECK(mat_equal(mat, dMat,TOLERANCE*10));
}

BOOST_AUTO_TEST_CASE(testObjectSpace)
{
	object_space os;

	os.location = vector3(100, 0, 0);
	os.rotation = vector3(0, nPI/2, 0);
	os.scale = vector3(1,1,1);
	os.update_matrix();

	vector3 p(0,0,0);
	vector3 t = p * os.local_to_world;
}