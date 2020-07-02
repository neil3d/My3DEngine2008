#include "StdAfx.h"
#include "basic_terrain_test.h"
#include "../../NexusNative/nengine/sky/sky_base.h"
#include "../../NexusNative/nengine/actor/nwater_component.h"
#include <boost/lexical_cast.hpp>

nDEFINE_CLASS(basic_terrain_test, nrender_test)

basic_terrain_test::basic_terrain_test(void)
{
}

basic_terrain_test::~basic_terrain_test(void)
{
}

nterrain_actor::ptr _generate_terrain(nlevel::ptr level_ptr)
{
	nterrain_actor::ptr trn = level_ptr->create_actor_t<nterrain_actor>(_T("test_actor"));

	const int trn_w = 256+1;
	const int trn_h = 256+1;	

	//-- 测试基本的地形创建流程
	nrect trn_rc(0,0,trn_w,trn_h);
	trn->create(trn_w, trn_h, 10, 65);
	trn->move(vector3(-128*trn_w/2,0,-128*trn_h/2), vector3(128, 2, 128));
	trn->generate_noise(trn_rc, 8, 512, 8);

	//-- 测试基本材质
	nterrain_mtl_setup* trn_mtl = trn->get_material();
	trn_mtl->create( resource_location(_T("demo_data:terrain/terrain.jpg")), -1, -1);

	layer_data layer;
	layer.uv_scale = vector2(32,32);
	layer.diffuse_tex  = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/dif_layer0.dds")));
//	layer.normal_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/normal_layer0.dds")));
	trn_mtl->add_layer(layer);

	layer.uv_scale = vector2(16,16);
	layer.diffuse_tex  = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/dif_layer1.dds")));
//	layer.normal_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/normal_layer1.dds")));
	layer.spec = 0.5;
	layer.spec_power = 2;
	trn_mtl->add_layer(layer);

	layer.uv_scale = vector2(16,16);
	layer.diffuse_tex  = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/dif_layer2.dds")));
	layer.normal_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/normal_layer2.dds")));
	layer.spec = 0;
	layer.spec_power = 0;
	trn_mtl->add_layer(layer);

	layer.uv_scale = vector2(16,16);
	layer.diffuse_tex  = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/dif_layer3.dds")));
	layer.normal_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/normal_layer3.dds")));
	trn_mtl->add_layer(layer);

	layer.uv_scale = vector2(8,8);
	layer.diffuse_tex  = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/dif_layer4.dds")));
	layer.normal_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("demo_data:terrain/normal_layer4.dds")));
	trn_mtl->add_layer(layer);

	trn_mtl->generate_noise(trn_mtl->get_layer_name(1), nrect(0,0,-1,-1), 8, 256, 8);
	trn_mtl->generate_noise(trn_mtl->get_layer_name(2), nrect(0,0,-1,-1), 8, 128, 8);
	trn_mtl->generate_noise(trn_mtl->get_layer_name(3), nrect(0,0,-1,-1), 8, 128, 8);
	trn_mtl->generate_noise(trn_mtl->get_layer_name(4), nrect(0,0,-1,-1), 8, 64, 8);

	//创建地表装饰层
#if 0
	nterrain_deco_setup* trn_deco = trn->get_deco();
	nterrain_static_mesh_deco_layer::ptr deco_layer=boost::shared_dynamic_cast<nterrain_static_mesh_deco_layer>(trn_deco->add_deco_layer(deco_layer_static_mesh,256));
	deco_layer->set_density(1.0f);
	deco_layer->set_random_offset(100.0f);
	deco_layer->set_max_random_scale(10.0f);
	deco_layer->set_min_random_scale(1.0f);
	deco_layer->set_random_rotation(180.0f);
	deco_layer->set_resource_loc(resource_location(L"demo_data",L"test/plane.nmdl"));
	deco_layer->do_test();	
#else
	nterrain_deco_setup* trn_deco = trn->get_deco();
	nterrain_grass_deco_layer::ptr deco_layer=boost::shared_dynamic_cast<nterrain_grass_deco_layer>(trn_deco->add_deco_layer(deco_layer_grass,64));
	deco_layer->set_density(0.05f);
	deco_layer->set_random_offset(100.0f);
	deco_layer->set_width(200);
	deco_layer->set_height(300);
	deco_layer->set_random_rotation(180.0f);
	deco_layer->set_resource_loc(resource_location(L"demo_data",L"test/grass.mtl"));
	deco_layer->do_test();	
	trn->_post_terrain_layer_change();
#endif
	
	
	return trn;
}

nmtl_static::ptr create_material(const nstring& material_name)
{
	//-- 创建我们的材质
	nmtl_static::ptr mtl( new nmtl_static(material_name) );

	mtl->create_from_hlsl(material_name, resource_location(
		_T("engine_data"),
		nstring(_T("material/")) + material_name + nstring(_T(".hlsl")))
		);

	mtl->set_cube_map_parameter("MTL_EnvMap", resource_location(_T("demo_data:unit_test/sky_cube.dds")));
	mtl->set_texture2d_parameter("MTL_NormalMap", resource_location(_T("demo_data:unit_test/WaterNormal.jpg")));
	mtl->set_value_parameter("MTL_AlphaRef",vector4(0.7f,0,0,0));
	mtl->set_blend_type( ETranslucent );
	return mtl;
}

void basic_terrain_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	vector3 dir(-1, -1,-1);
	m_light_anim_radius = 10000;
	add_direct_light(vec_normalize(dir));
	

	//--
	nterrain_actor::ptr trn = _generate_terrain(m_level_ptr);
	

	//-- 测试挖洞功能
	nrect trn_rc(10,10,28,28);
	int c = 0;
	nbit_map* viz = trn->get_viz_map();
	for(int y=10; y<18; y++)
	{
		for(int x=10; x<18; x++)
		{
			viz->set_value(x, y, false);
			c++;
		}
	}
	trn->post_vizmap_change(trn_rc);

#ifdef ENABLE_LEVEL_SAVE_TEST	// 顺便测试一下level读盘存盘
	resource_location lv_loc(_T("game_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif

	//--
	m_trn = trn;

	nmtl_static::ptr mtl( new nmtl_static(_T("test_mtl_st")) );

	mtl->create_from_hlsl(_T("DiffuseTexture"), resource_location(
		_T("engine_data"),
		_T("material/SKY_dome.hlsl"))
		);

	mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/vue_sky_001.jpg")));
	mtl->set_two_side(true);
	mtl->set_blend_type(EOpaque);

	//-- 创建mesh资源
	nresource_static_mesh::ptr mesh_ptr = 
		nresource_manager::instance()->load_static_mesh(
		resource_location(_T("demo_data:unit_test/material_ball.nmdl")) );

	mesh_ptr->import_set_material(0, 0, mtl);

	//-- 向level中添加actor
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("sky"));
	shared_ptr<nsky_component> comp_ptr 
		= actor_ptr->create_component_t<nsky_component>(_T("sky_comp"));
	comp_ptr->reset_resource(mesh_ptr);	
	actor_ptr->move(vector3::zero,vector3::zero,vector3(3800,3800,3800));


	//-- 创建我们的材质
	/*nmtl_static::ptr mtl( new nmtl_static(_T("test_mtl_st")) );

	mtl->create_from_hlsl(_T("DiffuseTexture"), resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl"))
		);

	mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:paint_whitemetal_df_.dds")));

	int index = 0;
	for (int i = -4; i < 4; i++)
	{
		for (int j = -4; j < 4; j ++)
		{
			add_sphere(nstring(_T("Sphere"))+boost::lexical_cast<std::wstring>(index++),vector3(i*2000,1500,j*2000),mtl);
		}
	}*/

	//水面
	nactor::ptr water_ptr = m_level_ptr->create_actor(_T("water"));
	shared_ptr<nwater_component> water_comp_ptr 
		= water_ptr->create_component_t<nwater_component>(_T("water_comp"));

	water_comp_ptr->move(vector3(300,500,-300),vector3::zero,vector3(10,10,10));
}

void basic_terrain_test::debug_draw(nrender_primitive_draw_interface* PDI)
{
}

void basic_terrain_test::close()
{
	nrender_test::close();
	m_trn.reset();
}