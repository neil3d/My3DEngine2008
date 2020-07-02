#include <boost/test/unit_test.hpp>
namespace unit = boost::unit_test;

unit::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	// Boost.Test操作
	unit::test_suite* test = BOOST_TEST_SUITE( "NexusEngine conosle test suite" );
	return test;
}

#include "../../NexusNative/nengine/engine_define.h"
#include "../../NexusNative/nengine//framework/nengine.h"
using namespace nexus;

namespace nexus
{
	class nengine_unit_test
	{
	public:
		void test_plugins(nengine& eg)
		{
			size_t num_plugin = eg.load_plugins();

			BOOST_CHECK(num_plugin > 0);

			//-- 检测plugin的class是否正确注册
			const nclass* render_class = nclass_factory::instance()->find_class(_T("nrenderer_d3d9"));
			BOOST_CHECK( render_class != NULL);
		}
	};
}

BOOST_AUTO_TEST_CASE(test_engine_init)
{
	nengine_unit_test engine_test;
	nengine my_engine;
	engine_test.test_plugins(my_engine);	
}