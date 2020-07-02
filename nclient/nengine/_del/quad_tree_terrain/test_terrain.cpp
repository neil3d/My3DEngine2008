#include "StdAfx.h"
#include "test_terrain.h"

nDEFINE_CLASS(test_terrain, nrender_test)

test_terrain::test_terrain(void)
{
}

test_terrain::~test_terrain(void)
{
}

void test_terrain::open(editor_camera_controller* cam)
{
	//-- 
	nrender_test::open(cam);
	add_point_light();

	//--
	nactor::ptr my_actor = m_level_ptr->create_actor(_T("my_terrain_actor"));

	nactor_component::ptr my_comp = my_actor->create_component(_T("my_terrain_comp"), _T("nquad_tree_terrain"));
	nterrain_base::ptr my_trn = boost::shared_dynamic_cast<nterrain_base>(my_comp);

	const int trn_w = 128+1;
	const int trn_h = 128+1;

	terrain_cfg cfg;
	cfg.enable_lod = true;
	cfg.scale = vector3(20, 20 ,20);

	my_trn->create(trn_w, trn_h, 0, cfg);
	my_trn->generate_noise(nrect(0,0,trn_w,trn_h), 8, 20, 2);

	//--
	vector3 center(trn_w/2*cfg.scale.x, 100, trn_h/2*cfg.scale.z);	
	cam->set_look_at(center);	
}
