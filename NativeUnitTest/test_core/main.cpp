
#include <time.h>
#include "d3dx10math.h"
#ifdef _DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif

#include <boost/test/unit_test.hpp>
namespace unit = boost::unit_test;

#include "ncore.h"


unit::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	nexus_init_core();
	// 一些初始化操作
	srand((unsigned int)time(NULL));
	D3DXCpuOptimizations(FALSE);

	// Boost.Test操作
	unit::test_suite* test = BOOST_TEST_SUITE( "NexusCore.Math test suite" );
	return test;
}