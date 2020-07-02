#include "StdAfx.h"
#include "test_terrain_collision.h"


nDEFINE_CLASS(test_terrain_collision, nphys_test)

test_terrain_collision::test_terrain_collision(void)
{
}

test_terrain_collision::~test_terrain_collision(void)
{
}

void test_terrain_collision::open(perspective_camera_controller* cam)
{
	nphys_test::open(cam);
	
	vector3 dir(50, 50,50);
	add_direct_light(vec_normalize(dir));

	nterrain_actor::ptr trn = m_level_ptr->create_actor_t<nterrain_actor>(_T("test_actor"));

	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	trn->create(trn_w, trn_h, 0, 65);

	trn->generate_noise(nrect(0,0,trn_w,trn_h), 8, 64, 2);
	vector3 scale(32, 64, 32);

	trn->move(vector3::zero, scale);
	/*trn->create_material_basic(resource_location(_T("game_data"),_T("terrain/terrain.jpg"))
		);	*/

	//--
	cam->set_look_at( trn->tile2world(trn_w/2, trn_h/2) );

	// begin play
	nengine::instance()->begin_play();
}