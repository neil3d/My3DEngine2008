#include "StdAfx.h"
#include "tet_speed_tree_collision.h"

nDEFINE_CLASS(tet_speed_tree_collision, nphys_test)

tet_speed_tree_collision::tet_speed_tree_collision(void)
	:m_st_index(0)
{
}

tet_speed_tree_collision::~tet_speed_tree_collision(void)
{
}

void tet_speed_tree_collision::load_spt( const nstring& str_res, const vector3& pos, float scale)
{
	std::wostringstream os;
	os << _T("speed_tree_actor") << ++m_st_index;

	nactor::ptr actor_ptr = m_level_ptr->create_actor(os.str());
	nspeed_tree_component::ptr spt_comp = actor_ptr->create_component_t<nspeed_tree_component>(_T("spt_comp"));

	spt_comp->create(resource_location(str_res));

	actor_ptr->move(pos, vector3::zero, vector3(scale,0,0));	
}

void tet_speed_tree_collision::open(perspective_camera_controller* cam)
{
	nphys_test::open(cam);

	//load_spt(_T("game_data:spt/BambooBush_RT.spt"),		vector3(m_st_index*100.f,0,0), 3);
	//load_spt(_T("game_data:spt/Ficus_Cluster.spt"),		vector3(m_st_index*100.f,0,0), 3);
	//load_spt(_T("game_data:spt/Frailejon_Cluster.spt"),	vector3(m_st_index*100.f,0,0), 3);
	//load_spt(_T("game_data:spt/SpeedTreeCAD1.spt"),		vector3(m_st_index*100.f,0,0), 3);
	load_spt(_T("game_data:spt/Kapok_RT.spt"),			vector3(m_st_index*100.f,0,0), 3);
	load_spt(_T("game_data:spt/MimosaTree_RT.spt"),		vector3(m_st_index*100.f,0,0), 3);
	//load_spt(_T("game_data:spt/MonkeyPuzzle_RT.spt"),	vector3(m_st_index*100.f,0,0), 3);

	vector3 dir(50, 50,50);
	add_direct_light(vec_normalize(dir));

	// begin play
	nengine::instance()->begin_play();
}
