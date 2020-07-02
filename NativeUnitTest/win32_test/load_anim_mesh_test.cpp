#include "StdAfx.h"
#include "load_anim_mesh_test.h"

nDEFINE_CLASS(load_anim_mesh_test, nresource_test)

load_anim_mesh_test::load_anim_mesh_test(void)
{
}

load_anim_mesh_test::~load_anim_mesh_test(void)
{
}


void load_anim_mesh_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	nresource_anim_mesh::ptr res = nresource_manager::instance()->load_anim_mesh(
		resource_location(_T("content:Misc_pile_001_02.nmdl")));
	
	//temp:Çå¿ÕÏûÏ¢
	nresource_manager::instance()->dispatch_events();

	nactor::ptr act = m_level_ptr->create_actor(_T("anim_test_actor"));
	shared_ptr<nanim_mesh_component> comp = act->create_component_t<nanim_mesh_component>(_T("mesh_comp"));
	comp->reset_resource(res);
	
	comp->set_anim(_T("run"));
	comp->play_anim(true, 1.0f, 0.f);

	//--
	cam->set_look_at(vector3::zero);
	m_light_anim_radius = 500;
}