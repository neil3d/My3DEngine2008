#include <boost/test/unit_test.hpp>
namespace unit = boost::unit_test;

#include "../../NexusNative/nengine/engine_define.h"
#include "../../NexusNative/nengine/framework/nengine.h"
#include "../../NexusNative/nengine/image/texture_atlas_creator.h"
using namespace nexus;

BOOST_AUTO_TEST_CASE(test_texture_atlas_creator)
{
	nimage::init_image_sdk();
	texture_atlas_creator tac;
	//int ret = tac.make_atlas(_T("../nexus_game/demo_data/ui"), 1024, 1024);
	//int ret = tac.make_atlas(_T("D:\\TestUI"), 2048, 2048);	
}