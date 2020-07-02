#include "StdAfx.h"
#include "import_static_mesh_test.h"
#include <boost/lexical_cast.hpp>

nDEFINE_CLASS(import_obj_test, nresource_test)
nDEFINE_CLASS(import_fsm_test, nresource_test)

nmtl_base::ptr _load_gow_man_mtl()
{
	nmtl_static::ptr mtl( new nmtl_static(_T("test_mtl_st")) );

	mtl->create_from_hlsl(_T("DiffuseTexture"), resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl"))
		);

	
	mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/obj/House_vil_001_03_D.tga")));
	//mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/gow/TEX_ManDiffuse.dds")));
	//mtl->set_texture2d_parameter("MTL_NormalMap", resource_location(_T("demo_data:unit_test/gow/TEX_ManNormal.dds")));

	return mtl;
}

void import_fsm_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	//-- 创建mesh资源
	nresource_static_mesh::ptr mesh_ptr(
		nresource_manager::instance()->new_static_mesh(_T("gow_man_mesh"))
		);

	mesh_ptr->import_lod(-1, _T("../nexus_game/demo_data/unit_test/gow/man.fsm"));
	mesh_ptr->import_set_material(0, 0, _load_gow_man_mtl());
	mesh_ptr->post_edit_change(true);

	//-- 向level中添加actor
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("gow_man_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);	

	//-- 
	cam->set_look_at(vector3::zero);
	m_light_anim_radius = 500;

	//--
	mesh_ptr->save_to_file(resource_location(_T("demo_data:unit_test/gow/man.nmdl")), true);
}

void import_obj_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	//-- 创建mesh资源
	nresource_static_mesh::ptr mesh_ptr(
		nresource_manager::instance()->new_static_mesh(_T("gow_man_mesh"))
		);

	mesh_ptr->import_lod(-1, _T("../nexus_game/demo_data/unit_test/obj/font.obj"));

	nmtl_static::ptr mtl1( new nmtl_static(_T("test_mtl_st_01")) );

	mtl1->create_from_hlsl(_T("DiffuseTexture"), resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl"))
		);

	mtl1->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/obj/font.bmp")));
	mesh_ptr->import_set_material(0, 0, mtl1);
	//nmtl_static::ptr mtl2( new nmtl_static(_T("test_mtl_st_02")) );

	//mtl2->create_from_hlsl(_T("DiffuseTexture"), resource_location(
	//	_T("engine_data"),
	//	_T("material/diffuse_basic.hlsl"))
	//	);
	//mtl2->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/obj/cup.jpg")));

	//nmtl_static::ptr mtl3( new nmtl_static(_T("test_mtl_st")) );

	//mtl3->create_from_hlsl(_T("DiffuseTexture"), resource_location(
	//	_T("engine_data"),
	//	_T("material/diffuse_basic.hlsl"))
	//	);
	//mtl3->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/obj/House_vil_001_03_D.tga")));

	
	//mesh_ptr->import_set_material(0, 1, mtl2);
	//mesh_ptr->import_set_material(0, 2, mtl3);
	mesh_ptr->post_edit_change(true);


	int index = 0;
	int count = 1;
	float dist = 512.0f;
	float offset = (count - 1)*dist/2; 
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j ++)
		{
			for (int k = 0; k < count; k++)
			{
				//-- 向level中添加actor
				nactor::ptr actor_ptr = m_level_ptr->create_actor(nstring(_T("cup"))+boost::lexical_cast<std::wstring>(index++));
				shared_ptr<nstatic_mesh_component> comp_ptr 
					= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
				comp_ptr->reset_resource(mesh_ptr);	
				actor_ptr->move(vector3(i*dist - offset,k*dist,j*dist - offset),vector3::zero,vector3(100,100,100));
			}			
		}
	}

	//-- 
	cam->set_look_at(vector3::zero);
	m_light_anim_radius = 500;

	//--
	mesh_ptr->save_to_file(resource_location(_T("demo_data:unit_test/gow/man.nmdl")), true);
}