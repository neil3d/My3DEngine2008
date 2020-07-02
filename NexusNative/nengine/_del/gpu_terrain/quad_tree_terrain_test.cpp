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

void quad_tree_terrain_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	vector3 dir(50, 50,50);
	add_direct_light(vec_normalize(dir));

	//--
#if 1
	nterrain_actor::ptr trn = m_level_ptr->create_actor_t<nterrain_actor>(_T("test_actor"));
	
	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	trn->create(trn_w, trn_h, 0, 65);
#if 0
	trn->generate_noise(nrect(0,0,trn_w,trn_h), 8, 64, 2);
	vector3 scale(128, 64, 128);
#else
	trn->import_heightmap(_T("../game_data/terrain/heightMap.bmp"));
	vector3 scale(128, 48, 128);
#endif

	trn->move(vector3::zero, scale);
	trn->create_material_basic(resource_location(_T("game_data"),_T("terrain/terrain.jpg"))
		);	

	//--
	cam->set_look_at( trn->tile2world(trn_w/2, trn_h/2) );

#if 0	// 是否打开地形水面测试代码
	trn->create_water(115, 65);
	//trn->create_water_material_basic(resource_location(_T("game_data"),_T("terrain/water.jpg")) );
	trn->create_water_material_natural(256, 256,
		resource_location(_T("game_data:terrain/water.jpg")),
		resource_location(_T("game_data:terrain/waterbump.jpg"))
		);
#endif

#endif

#if 1	// 是否打开Sky Box测试代码
	nsky_actor::ptr sky = m_level_ptr->create_actor_t<nsky_actor>(_T("test_sky"));
	sky->create_sky_box( resource_location(_T("game_data:sky_cube.dds")) );
	//sky->create_sky_dome(18000, 50);
#endif

	//加载几颗SpeedTree
	for(int i=0;i<20;++i)
	{
		wostringstream os;
		os << _T("speed_tree_actor")
			<< i;

		nactor::ptr actor_ptr = m_level_ptr->create_actor(os.str());
		nspeed_tree_component::ptr spt_comp = actor_ptr->create_component_t<nspeed_tree_component>(_T("spt_comp"));


		switch(rand()%5)
		{
		case 0:
			{
				spt_comp->create(resource_location(_T("game_data:spt/Kapok_RT.spt")));
				break;
			}
		case 1:
			{
				spt_comp->create(resource_location(_T("game_data:spt/MonkeyPuzzle_RT.spt")));
				break;
			}
		case 2:
			{
				spt_comp->create(resource_location(_T("game_data:spt/BambooBush_RT.spt")));
				break;
			}
		case 3:
			{
				spt_comp->create(resource_location(_T("game_data:spt/Ficus_Cluster.spt")));
				break;
			}
		case 4:
			{
				spt_comp->create(resource_location(_T("game_data:spt/Frailejon_Cluster.spt")));
				break;
			}
		}
		//spt_comp->create(resource_location(_T("game_data:spt/BambooBush_RT.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/Ficus_Cluster.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/Frailejon_Cluster.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/HydrillaCluster_RT.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/Kapok_RT.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/MimosaTree_RT.spt")));
		//spt_comp->create(resource_location(_T("game_data:spt/MonkeyPuzzle_RT.spt")));

		vector3 pos(rand()%256*128,1024,rand()%256*128);
		actor_ptr->move(pos, vector3::zero, vector3(100,0,0));	
	}

#ifdef ENABLE_LEVEL_SAVE_TEST	// 顺便测试一下level读盘存盘
	resource_location lv_loc(_T("game_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//	class terrain_tex_splatting_test
/////////////////////////////////////////////////////////////////////////////
terrain_tex_splatting_test::terrain_tex_splatting_test(void)
{}

terrain_tex_splatting_test::~terrain_tex_splatting_test(void)
{}

extern nresource_static_mesh::ptr _load_man_mesh();

nterrain_actor::ptr _load_terrain(nlevel::ptr level_ptr)
{
	nterrain_actor::ptr trn = level_ptr->create_actor_t<nterrain_actor>(_T("test_terrain_actor"));

	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	trn->create(trn_w, trn_h, 0, 129);
	trn->import_heightmap(_T("../game_data/terrain/heightMap.bmp"));	
	vector3 scale(128, 24, 128);
	trn->move(vector3::zero, scale);

	//-- create material
	ntexture_splatting::ptr mtl = trn->create_texture_splatting(trn_w-1, trn_h-1);
	
#if 1
	mtl->set_layer_texture(0, resource_location(_T("game_data:terrain/tex_splat/grass.jpg")));
	mtl->set_layer_texture(1, resource_location(_T("game_data:terrain/tex_splat/scorched.jpg")));
	mtl->set_layer_texture(2, resource_location(_T("game_data:terrain/tex_splat/patchy.jpg")));
	mtl->set_layer_texture(3, resource_location(_T("game_data:terrain/tex_splat/rock.jpg")));
	mtl->set_layer_texture(4, resource_location(_T("game_data:terrain/tex_splat/stone.jpg")));	
#else
	mtl->set_layer_texture(0, resource_location(_T("demo_data:terrain/dif_layer0.dds")));
	mtl->set_layer_texture(1, resource_location(_T("demo_data:terrain/dif_layer1.dds")));
	mtl->set_layer_texture(2, resource_location(_T("demo_data:terrain/dif_layer2.dds")));
	mtl->set_layer_texture(3, resource_location(_T("demo_data:terrain/dif_layer3.dds")));
	mtl->set_layer_texture(4, resource_location(_T("demo_data:terrain/dif_layer4.dds")));	
#endif

	mtl->import_layer_alpha(1, _T("../game_data/terrain/tex_splat/MyMap_layer_1.bmp"));
	mtl->import_layer_alpha(2, _T("../game_data/terrain/tex_splat/MyMap_layer_2.bmp"));
	mtl->import_layer_alpha(3, _T("../game_data/terrain/tex_splat/MyMap_layer_3.bmp"));
	mtl->import_layer_alpha(4, _T("../game_data/terrain/tex_splat/MyMap_layer_4.bmp"));

	return trn;
}

void _add_test_man_mesh(nlevel::ptr level_ptr, vector3 pos)
{
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();

	nactor::ptr actor_ptr = level_ptr->create_actor(_T("gow_man_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);		

	pos.y += comp_ptr->get_bounds().sphere_radius;
	actor_ptr->move(pos, vector3::zero, vector3::one);
}

void _load_spt(nlevel::ptr level_ptr, const vector3& pos, float scale);
void terrain_tex_splatting_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	vector3 dir(50, 100,50);
	add_direct_light(vec_normalize(dir));

	//-- create terrain
	nterrain_actor::ptr trn = _load_terrain(m_level_ptr);

	//--
	vector3 look_at = trn->tile2world(104, 99);
	cam->set_look_at( look_at );

#if 0 // 为了测试阴影，临时加个mesh actor
	_add_test_man_mesh(m_level_ptr, look_at);

	vector3 tree_pos = look_at;
	tree_pos.x -= 168;
	_load_spt(m_level_ptr, tree_pos, 8);
#endif

#ifdef ENABLE_LEVEL_SAVE_TEST	// 顺便测试一下level读盘存盘
	resource_location lv_loc(_T("game_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif
}