#include "StdAfx.h"
#include "load_skeletal_mesh_animset_test.h"

nDEFINE_CLASS(load_skeletal_mesh_animset_test, nresource_test)

load_skeletal_mesh_animset_test::load_skeletal_mesh_animset_test(void)
{
}

load_skeletal_mesh_animset_test::~load_skeletal_mesh_animset_test(void)
{
}

void load_skeletal_mesh_animset_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	//-- ÏòlevelÖÐÌí¼Óactor
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("TEST_SKEL_ANIM"));
	shared_ptr<nskeletal_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nskeletal_mesh_component>(_T("skel_mesh_comp"));

	//comp_ptr->reset_resource( resource_location(_T("demo_data:unit_test/npc01/npc.skm")),true);	

	comp_ptr->reset_resource( resource_location(_T("content:characters/XiaShi/body.skm")),true);	

	//-- anim set
	nresource_skeletal_anim_set::ptr anim_set 
		//= nresource_manager::instance()->load_skeletal_anim_set( resource_location(_T("demo_data:unit_test/npc01/npc.animset")) );
		= nresource_manager::instance()->load_skeletal_anim_set( resource_location(_T("content:characters/XiaShi/base_act.animset")) );

	shared_ptr<nskel_anim_play> anim_ctrl = dynamic_pointer_cast<nskel_anim_play>
		( comp_ptr->reset_anim_controller(_T("nskel_anim_play")) );
	anim_ctrl->init(anim_set, true);


	actor_ptr->move(vector3(100,50,25), vector3(0,nPI/4,0), vector3::one);

	//-- 
	cam->set_look_at(vector3::zero);
	m_light_anim_radius = 500;
}