#include "StdAfx.h"
#include "level_load_test.h"
#include "timer/nprofile.h"

nDEFINE_CLASS(level_load_test, nrender_test)

level_load_test::level_load_test(void)
{
}

level_load_test::~level_load_test(void)
{
}

void level_load_test::open(perspective_camera_controller* cam)
{
	//resource_location loc(_T("demo_data"), _T("level"));
	//resource_location loc(_T("maps"), _T("m01"));
	resource_location loc(_T("maps"), _T("YuZhou"));
	m_level_ptr = nengine::instance()->load_level(loc);
}

void level_load_test::debug_draw( nrender_primitive_draw_interface* PDI )
{
	profile_value* tw_value = profile_manager::instance()->find_profile(nname(_T("octree_scene")));
	if(tw_value)
	{
		PDI->draw_debug_string(10, 50,tw_value->to_string(), color4ub(0,255,0,255));
	}
}