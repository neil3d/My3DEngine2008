#include "StdAfx.h"
#include "import_fak_test.h"

nDEFINE_CLASS(import_fak_test, nresource_test)

import_fak_test::import_fak_test(void)
{
}

import_fak_test::~import_fak_test(void)
{
}

void _load_Arthas_mtl(std::vector<nmtl_base::ptr>& ret)
{
	// make material	
	resource_location shader_loc(_T("engine_data"), _T("material/diffuse_basic.hlsl"));

	//--
	nmtl_static::ptr mtl_a( new nmtl_static(_T("ArthasMtlA")) );

	mtl_a->create_from_hlsl(_T("DiffuseTexture"), shader_loc);

	mtl_a->set_texture2d_parameter("MTL_DiffuseMap", 
		resource_location(_T("demo_data:unit_test/Arthas/ARTHASLICHKING_01.tga"))
		);

	//--
	nmtl_static::ptr mtl_b( new nmtl_static(_T("ArthasMtlA")) );

	mtl_b->create_from_hlsl(_T("DiffuseTexture"), shader_loc);

	mtl_b->set_texture2d_parameter("MTL_DiffuseMap", 
		resource_location(_T("demo_data:unit_test/Arthas/ARTHASLICHKING_02.tga"))
		);

	//--
	ret.push_back(mtl_a);
	ret.push_back(mtl_b);
}


void import_fak_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	//-- 创建资源，导入fak文件
	nresource_anim_mesh::ptr mesh_ptr( 
		nresource_manager::instance()->new_anim_mesh(_T("anim_Arthas"))
		);

	mesh_ptr->import_lod(0, _T("../nexus_game/demo_data/unit_test/Arthas/Arthas.fak"));

	std::vector<nmtl_base::ptr> mtl_array;
	_load_Arthas_mtl(mtl_array);
	mesh_ptr->import_set_material(0, 0, mtl_array[0]);
	mesh_ptr->import_set_material(0, 1, mtl_array[0]);
	mesh_ptr->import_set_material(0, 2, mtl_array[1]);
	mesh_ptr->import_set_material(0, 3, mtl_array[1]);

	mesh_ptr->post_edit_change(true);


	//-- 创建Actor
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("anim_Arthas_actor"));
	shared_ptr<nanim_mesh_component> comp_ptr = actor_ptr->create_component_t<nanim_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);

	actor_ptr->move(vector3::zero, vector3::zero, vector3(100,100,100));

	//-- 
	cam->set_look_at(vector3::zero);
	m_light_anim_radius = 500;
}