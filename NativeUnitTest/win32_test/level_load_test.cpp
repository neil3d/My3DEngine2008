#include "StdAfx.h"
#include "level_load_test.h"

nDEFINE_CLASS(level_load_test, nrender_test)

level_load_test::level_load_test(void)
{
}

level_load_test::~level_load_test(void)
{
}

void level_load_test::open(perspective_camera_controller* cam)
{
	resource_location loc(_T("game_data"), _T("level"));
	m_level_ptr = nengine::instance()->load_level(loc);
}