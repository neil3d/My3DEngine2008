#include <boost/test/unit_test.hpp>

namespace unit = boost::unit_test;

unit::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	// Boost.Test²Ù×÷
	unit::test_suite* test = BOOST_TEST_SUITE( "NexusCore.Math test suite" );
	return test;
	}
