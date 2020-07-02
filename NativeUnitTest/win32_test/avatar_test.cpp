#include "StdAfx.h"
#include "avatar_test.h"

nDEFINE_CLASS(avatar_test, nrender_test)

avatar_test::avatar_test(void)
{
}

avatar_test::~avatar_test(void)
{
}

void avatar_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);

	nactor::ptr act = m_level_ptr->create_actor(_T("anim_test_actor"), _T("navatar_actor"));
	navatar_actor::ptr avatar = boost::dynamic_pointer_cast<navatar_actor>(act);
	avatar->create(resource_location(_T("content:characters/XiaShi/base_act.animset")));

	avatar->bind_skeletal_mesh( _T("chest"),
		resource_location(_T("content:characters/XiaShi/initial_skin/chest.skm")) );

	avatar->bind_skeletal_mesh( _T("face"),
		resource_location(_T("content:characters/XiaShi/initial_skin/face.skm")) );

	avatar->bind_skeletal_mesh( _T("feet"),
		resource_location(_T("content:characters/XiaShi/initial_skin/feet.skm")) );

	avatar->bind_skeletal_mesh( _T("hand"),
		resource_location(_T("content:characters/XiaShi/initial_skin/hand.skm")) );

	avatar->bind_skeletal_mesh( _T("legs"),
		resource_location(_T("content:characters/XiaShi/initial_skin/legs.skm")) );

	avatar->bind_skeletal_mesh( _T("shoulders"),
		resource_location(_T("content:characters/XiaShi/initial_skin/shoulders.skm")) );

	avatar->bind_skeletal_mesh( _T("hat"),
		resource_location(_T("content:characters/XiaShi/initial_skin/hat.skm")) );

	avatar->bind_attachment( _T("weapon"), _T("objpolySurface1"),
		resource_location(_T("content:equip/weapon/init/init.nmdl")) );

	//-- 设置动画
	avatar->set_anim(_T("run"));
	avatar->play_anim(true, 1, 0);
	avatar->move(vector3(100,0,100));

	////-- 创建一个底板
	nactor::ptr box_ptr = m_level_ptr->create_actor(_T("Plane"));
	shared_ptr<nshape_component> comp_ptr = box_ptr->create_component_t<nshape_component>(_T("shape_comp"));
	float h = 20;
	comp_ptr->create_box(2000,h*2,2000);
	comp_ptr->set_material( create_test_material(_T("diffuse_basic")) );
	box_ptr->move(vector3(0,-h,0));
}