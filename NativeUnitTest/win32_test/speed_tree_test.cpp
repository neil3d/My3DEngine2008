#include "StdAfx.h"
#include "speed_tree_test.h"

nDEFINE_CLASS(speed_tree_test, nrender_test)

speed_tree_test::speed_tree_test(void)
{
}

speed_tree_test::~speed_tree_test(void)
{
}

void _load_spt(nlevel::ptr level_ptr, const vector3& pos, float scale)
{
	static int i = 0;
	wostringstream os;
	os << _T("speed_tree_actor")
		<< i;
	i++;

	nactor::ptr actor_ptr = level_ptr->create_actor(os.str());
	nspeed_tree_component::ptr spt_comp = actor_ptr->create_component_t<nspeed_tree_component>(_T("spt_comp"));


	//spt_comp->create(resource_location(_T("game_data:spt/BambooBush_RT.spt")));
	//spt_comp->create(resource_location(_T("game_data:spt/Ficus_Cluster.spt")));
	//spt_comp->create(resource_location(_T("game_data:spt/Frailejon_Cluster.spt")));
	//spt_comp->create(resource_location(_T("game_data:spt/HydrillaCluster_RT.spt")));
	spt_comp->create(resource_location(_T("game_data:spt/Kapok_RT.spt")));
	//spt_comp->create(resource_location(_T("game_data:spt/MimosaTree_RT.spt")));
	//spt_comp->create(resource_location(_T("game_data:spt/MonkeyPuzzle_RT.spt")));

	actor_ptr->move(pos, vector3::zero, vector3(scale,0,0));	
}

void speed_tree_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	_load_spt(m_level_ptr, vector3(0,0,0), 1);

#ifdef ENABLE_LEVEL_SAVE_TEST	// Ë³±ã²âÊÔÒ»ÏÂlevel¶ÁÅÌ´æÅÌ
	resource_location lv_loc(_T("game_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif

}