#include "StdAfx.h"
#include "quad_tree_terrain_test.h"

nDEFINE_CLASS(quad_tree_terrain_test, nrender_test)
nDEFINE_CLASS(terrain_tex_splatting_test, nrender_test)

/////////////////////////////////////////////////////////////////////////////
//	class quad_tree_terrain_test
/////////////////////////////////////////////////////////////////////////////
quad_tree_terrain_test::quad_tree_terrain_test(void)
{
}

quad_tree_terrain_test::~quad_tree_terrain_test(void)
{
}

void quad_tree_terrain_test::open(editor_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	nterrain_actor::ptr trn = m_level_ptr->create_actor_t<nterrain_actor>(_T("test_actor"));
	
	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	trn->create(trn_w, trn_h, 0, 17);
#if 1
	trn->generate_noise(nrect(0,0,trn_w,trn_h), 8, 256, 2);
	vector3 scale(128, 64, 128);
#else
	trn->import_heightmap(_T("../game_data/terrain/heightMap.bmp"));
	vector3 scale(128, 32, 128);
#endif

	trn->set_scale(scale);
	trn->create_material_basic(resource_location(_T("game_data"),_T("terrain/terrain.jpg"))
		);

	m_trn_ed.bind_terrain(trn);
	//--
	nexus::nrenderer_base* renderer = nexus::nengine::instance()->get_renderer();
	renderer->set_render_mode(nexus::ERM_Unlit);

	//--
	cam->set_look_at( trn->tile2world(trn_w/2, trn_h/2) );
}

/////////////////////////////////////////////////////////////////////////////
//	class terrain_tex_splatting_test
/////////////////////////////////////////////////////////////////////////////
terrain_tex_splatting_test::terrain_tex_splatting_test(void)
{}

terrain_tex_splatting_test::~terrain_tex_splatting_test(void)
{}

void terrain_tex_splatting_test::open(editor_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//-- create terrain
	nterrain_actor::ptr trn = m_level_ptr->create_actor_t<nterrain_actor>(_T("test_actor"));

	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	trn->create(trn_w, trn_h, 0, 17);
	trn->import_heightmap(_T("../game_data/terrain/heightMap.bmp"));	
	vector3 scale(128, 64, 128);
	trn->set_scale(scale);

	//-- create material
	ntexture_splatting::ptr mtl = trn->create_texture_splatting(trn_w-1, trn_h-1);
	mtl->set_layer_texture(0, resource_location(_T("game_data:terrain/tex_splat/grass.jpg")));
	mtl->set_layer_texture(1, resource_location(_T("game_data:terrain/tex_splat/scorched.jpg")));
	mtl->set_layer_texture(2, resource_location(_T("game_data:terrain/tex_splat/patchy.jpg")));
	mtl->set_layer_texture(3, resource_location(_T("game_data:terrain/tex_splat/rock.jpg")));
	mtl->set_layer_texture(4, resource_location(_T("game_data:terrain/tex_splat/stone.jpg")));	

	mtl->import_layer_alpha(1, _T("../game_data/terrain/tex_splat/MyMap_layer_1.bmp"));
	mtl->import_layer_alpha(2, _T("../game_data/terrain/tex_splat/MyMap_layer_2.bmp"));
	mtl->import_layer_alpha(3, _T("../game_data/terrain/tex_splat/MyMap_layer_3.bmp"));
	mtl->import_layer_alpha(4, _T("../game_data/terrain/tex_splat/MyMap_layer_4.bmp"));

	//--
	nexus::nrenderer_base* renderer = nexus::nengine::instance()->get_renderer();
	renderer->set_render_mode(nexus::ERM_Unlit);

	//--
	cam->set_look_at( trn->tile2world(trn_w/2, trn_h/2) );
}