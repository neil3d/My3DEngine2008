#include "ncore.h"
#include <boost/test/unit_test.hpp>
#include "ncore.h"
#include "object/serialize.h"
#include "object/narchive_xml.h"
#include "file/nstd_file_system.h"
#include "ngameframework.h"

using namespace std;
using namespace nexus;
using namespace gameframework;

BOOST_AUTO_TEST_CASE(test_nlevel_info_resource)
{
	//nstring root_dir = nfile_sys_default_root();
	//nstd_file_system file_sys;
	//file_sys.init(root_dir);

	//nlevel_info_resource info;

	//start_point_data new_start_point(_T("level_sp_1"));
	//new_start_point.radius = 1000.f;
	//info.add_info(new_start_point);

	//new_start_point.name.reset(info.make_unique_id(ELIRT_StartPoint));
	//info.add_info(new_start_point);

	//new_start_point.name.reset(info.make_unique_id(ELIRT_StartPoint));
	//info.add_info(new_start_point);

	//trigger_data new_trigger(_T("level_trigger_1"));
	//new_trigger.start_point = new_start_point.name;
	//info.add_info(new_trigger);

	//BOOST_CHECK(info.add_info(new_trigger) == false);

	//info.serialize(&file_sys, _T("demo_data"), _T("test/level_info_resource.xml"),EFileWrite, ELIRT_ALL);

	//nlevel_info_resource test_read_info;
	//test_read_info.serialize(&file_sys, _T("demo_data"), _T("test/level_info_resource.xml"),EFileRead, ELIRT_ALL);
	//
	//BOOST_CHECK(test_read_info.get_start_point_data().size() == 3);
	//BOOST_CHECK(test_read_info.get_trigger_data().size() == 1);
	//const trigger_data* trigger = test_read_info.find_trigger_data(nname(_T("level_trigger_1")));
	//BOOST_CHECK(trigger != NULL);
	//BOOST_CHECK(trigger->start_point == new_start_point.name);

	//::system("pause");
}


