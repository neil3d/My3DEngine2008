#include "ncore.h"
#include <boost/test/unit_test.hpp>
#include <d3dx10math.h>

using namespace nexus;

//-- Test vector3 --------------------------------------------------------------
bool operator ==(const vector3& nv, const D3DXVECTOR3& dv)
{
	return nv.x == dv.x
		&& nv.y == dv.y
		&& nv.z == dv.z;
}

// add
BOOST_AUTO_TEST_CASE(testVec3Add)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f),
			v2B(rand()/100.0f,rand()/100.0f, rand()/100.0f);
	vector3 v2C = v2A+v2B;

	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z), d3dV2B(v2B.x, v2B.y, v2B.z);
	D3DXVECTOR3 d3dV2C;
	D3DXVec3Add(&d3dV2C, &d3dV2A, &d3dV2B);

	BOOST_CHECK(v2C == d3dV2C);		
}

// subtract
BOOST_AUTO_TEST_CASE(testVec3Sub)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f),
		v2B(rand()/100.0f,rand()/100.0f, rand()/100.0f);
	vector3 v2C = v2A-v2B;

	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z), d3dV2B(v2B.x, v2B.y, v2B.z);
	D3DXVECTOR3 d3dV2C;

	D3DXVec3Subtract(&d3dV2C, &d3dV2A, &d3dV2B);
	BOOST_CHECK(v2C == d3dV2C);		
}

// scale
BOOST_AUTO_TEST_CASE(testVec3Scale)
{
	float s = rand()/10.0f;

	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f);
	vector3 v2C = v2A*s;

	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z);
	D3DXVECTOR3 d3dV2C;

	D3DXVec3Scale(&d3dV2C, &d3dV2A, s);

	BOOST_CHECK(v2C == d3dV2C);		
}

// length
BOOST_AUTO_TEST_CASE(testVec3Len)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f);
	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z);

	float len = vec_length(v2A);
	float d3dLen = D3DXVec3Length(&d3dV2A);
	
	BOOST_CHECK_EQUAL( len , d3dLen);
}

// normal
BOOST_AUTO_TEST_CASE(testVec3Normal)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f);
	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z);

	v2A = vec_normalize(v2A);
	D3DXVec3Normalize(&d3dV2A, &d3dV2A);

	BOOST_CHECK(v2A == d3dV2A);		
}

// dot
BOOST_AUTO_TEST_CASE(testVec3Dot)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f),
		v2B(rand()/100.0f,rand()/100.0f, rand()/100.0f);
	
	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z), d3dV2B(v2B.x, v2B.y, v2B.z);
	
	
	BOOST_CHECK_EQUAL(vec_dot(v2A, v2B),
		D3DXVec3Dot(&d3dV2A, &d3dV2B));		
}

// cross
BOOST_AUTO_TEST_CASE(testVec3Cross)
{
	vector3 v2A(rand()/100.0f, rand()/100.0f, rand()/100.0f),
		v2B(rand()/100.0f,rand()/100.0f, rand()/100.0f);
	vector3 v2C;

	D3DXVECTOR3 d3dV2A(v2A.x, v2A.y, v2A.z), d3dV2B(v2B.x, v2B.y, v2B.z);
	D3DXVECTOR3 d3dV2C;

	v2C = vec_cross(v2A, v2B);
	D3DXVec3Cross(&d3dV2C, &d3dV2A, &d3dV2B);

	BOOST_CHECK(v2C == d3dV2C);		
}