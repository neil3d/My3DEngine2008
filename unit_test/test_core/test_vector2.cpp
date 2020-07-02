#include "ncore.h"
#include <boost/test/unit_test.hpp>
#include <d3dx10math.h>

//#define USE_D3D_MATH
#define USE_NV_MATH
#include "nv_math/nv_math.h"

using namespace nexus;

//-- Test vector2 --------------------------------------------------------------
// compare with D3D
bool operator ==(const vector2& nv, const D3DXVECTOR2& dv)
{
	return nv.x == dv.x
		&& nv.y == dv.y;
}

// compare with nv_math
bool operator ==(const vector2& nv, const vec2& dv)
{
	return nv.x == dv.x
		&& nv.y == dv.y;
}

// add
BOOST_AUTO_TEST_CASE(testVec2Add)
{
	vector2 v2A(rand()/100.0f, rand()/100.0f), v2B(rand()/100.0f,rand()/100.0f);
	vector2 v2C = v2A+v2B;

#ifdef USE_NV_MATH
	vec2 nv1(v2A.x, v2A.y), nv2(v2B.x, v2B.y);
	vec2 nv3 = nv1+nv2;

	BOOST_CHECK(v2C == nv3);		
#endif

#ifdef USE_D3D_MATH
	D3DXVECTOR2 d3dV2A(v2A.x, v2A.y), d3dV2B(v2B.x, v2B.y);
	D3DXVECTOR2 d3dV2C;
	D3DXVec2Add(&d3dV2C, &d3dV2A, &d3dV2B);

	BOOST_CHECK(v2C == d3dV2C);		
#endif
}

// subtract
BOOST_AUTO_TEST_CASE(testVec2Sub)
{
	vector2 v2A(rand()/100.0f, rand()/100.0f), v2B(rand()/100.0f,rand()/100.0f);
	vector2 v2C = v2A-v2B;

#ifdef USE_NV_MATH
	vec2 nv1(v2A.x, v2A.y), nv2(v2B.x, v2B.y);
	vec2 nv3 = nv1-nv2;

	BOOST_CHECK(v2C == nv3);		
#endif

#ifdef USE_D3D_MATH
	D3DXVECTOR2 d3dV2A(v2A.x, v2A.y), d3dV2B(v2B.x, v2B.y);
	D3DXVECTOR2 d3dV2C;

	D3DXVec2Subtract(&d3dV2C, &d3dV2A, &d3dV2B);
	BOOST_CHECK(v2C == d3dV2C);		
#endif
}

// scale
BOOST_AUTO_TEST_CASE(testVec2Scale)
{
	float s = rand()/10.0f;

	vector2 v2A(rand()/100.0f, rand()/100.0f);
	vector2 v2C = v2A*s;

#ifdef USE_NV_MATH
	vec2 nv1(v2A.x, v2A.y);
	vec2 nv3 = s*nv1;

	BOOST_CHECK(v2C == nv3);		
#endif

#ifdef USE_D3D_MATH
	D3DXVECTOR2 d3dV2A(v2A.x, v2A.y);
	D3DXVECTOR2 d3dV2C;

	D3DXVec2Scale(&d3dV2C, &d3dV2A, s);

	BOOST_CHECK(v2C == d3dV2C);		
#endif
}

// length
BOOST_AUTO_TEST_CASE(testVec2Len)
{
	vector2 v2A(rand()/100.0f, rand()/100.0f);
	float len = vec_length(v2A);

#ifdef USE_NV_MATH
	vec2 nv1(v2A.x, v2A.y);
	
	BOOST_CHECK_EQUAL(len, nv1.norm());		
#endif

#ifdef USE_D3D_MATH
	D3DXVECTOR2 d3dV2A(v2A.x, v2A.y);
	float d3dLen = D3DXVec2Length(&d3dV2A);

	BOOST_CHECK_EQUAL( len , d3dLen);
#endif
}

// normal
BOOST_AUTO_TEST_CASE(testVec2Normal)
{
	vector2 v2A(rand()/100.0f, rand()/100.0f);
	D3DXVECTOR2 d3dV2A(v2A.x, v2A.y);

	v2A = vec_normalize(v2A);
	D3DXVec2Normalize(&d3dV2A, &d3dV2A);

	BOOST_CHECK(v2A == d3dV2A);		
}