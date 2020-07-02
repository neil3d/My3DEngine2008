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
	nchunked_terrain::ptr my_trn = m_level_ptr->create_actor_t<nchunked_terrain>(_T("my_terrain_actor"));
	
	const int trn_w = 256+1;
	const int trn_h = 256+1;	
	
	my_trn->create(trn_w, trn_h, 0, 65);
	
#if 0
	my_trn->generate_noise(nrect(0,0,trn_w,trn_h), 8, 20, 2);
	vector3 scale(128, 64, 128);
#else
	my_trn->import_heightmap(_T("../game_data/terrain/heightMap.bmp"));
	vector3 scale(128, 16, 128);
#endif

	my_trn->create_material_basic(resource_location(_T("../game_data"),_T("terrain/terrain.jpg"))
		);


	my_trn->set_scale(scale);
	

	//--	
	vector3 center(0, 0, 0);	
	cam->set_look_at(center);	

	//--
	nexus::nrenderer_base* renderer = nexus::nengine::instance()->get_renderer();
	renderer->set_render_mode(nexus::ERM_Unlit);
}
