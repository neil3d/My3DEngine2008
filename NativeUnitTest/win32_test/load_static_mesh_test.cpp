#include "StdAfx.h"
#include "load_static_mesh_test.h"
#include "../../NexusNative/nengine/sky/sky_base.h"

nDEFINE_CLASS(load_static_mesh_test, nresource_test)

load_static_mesh_test::load_static_mesh_test(void):nresource_test()
{
}

load_static_mesh_test::~load_static_mesh_test(void)
{
}

nmtl_base::ptr _load_mtl()
{
	nmtl_static::ptr mtl( new nmtl_static(_T("test_mtl_st")) );

	mtl->create_from_hlsl(_T("DiffuseTexture"), resource_location(
		_T("engine_data"),
		_T("material/SKY_dome.hlsl"))
		);

	mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/vue_sky_001.jpg")));
	mtl->set_two_side(true);
	return mtl;
}

void load_static_mesh_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);
	m_light_anim_radius = 1000;

	//-- 创建mesh资源
	nresource_static_mesh::ptr mesh_ptr = 
		nresource_manager::instance()->load_static_mesh(
		resource_location(_T("content:staticmesh/Misc_pile_001/Misc_pile_001_02.nmdl")) );
		
//	mesh_ptr->import_set_material(0, 0, _load_mtl());
	
	//-- 向level中添加actor
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("NPC_bandit_001"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);	
	actor_ptr->move(vector3::zero,vector3(0,0,0),vector3(10,10,10));
	//-- 
	cam->set_look_at(vector3::zero);

	//mesh_ptr->save_to_file(resource_location(_T("demo_data:unit_test/NPC_bandit_001/NPC_bandit_001.nmdl")), true);
}
