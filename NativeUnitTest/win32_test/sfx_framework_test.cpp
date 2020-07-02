#include "StdAfx.h"
#include "sfx_framework_test.h"

nDEFINE_CLASS(sfx_framework_test, nrender_test)

sfx_framework_test::sfx_framework_test(void)
{
}

sfx_framework_test::~sfx_framework_test(void)
{
}

neffect_element::ptr _test_effect_quad()
{
	//-- create a quad
	neffect_quad::ptr new_quad( new neffect_quad );
	new_quad->init(_T("TestSFXQuad"));

	//-- 设置静态数据
	new_quad->m_init_pos.x = 100;
	ndistribution_const<vector3>* rot_vel = new ndistribution_const<vector3>(vector3(0,0.707f,0.707f));
	new_quad->m_face_type = ESFX_SphereBillboard;
	new_quad->m_rotate_vel.reset(rot_vel);

	new_quad->m_num_tail_faces = 1;
	new_quad->m_tail_interval = 1.0f/16;

	//-- 设置动态数据
	ndistribution_curve<vector2>* size_curve = new ndistribution_curve<vector2>();
	size_curve->add_key(vector2(40,40),0);
	size_curve->add_key(vector2(40,40),2);
	size_curve->add_key(vector2(40,40),4);
	new_quad->m_size.reset(size_curve);

	return new_quad;
}

neffect_element::ptr _test_effect_grid()
{
	//-- 创建特效元素对象
	neffect_grid::ptr new_elem( new neffect_grid );
	new_elem->m_type=neffect_grid_sphere_billboard;
	new_elem->init(_T("neffect_grid"));	
	return new_elem;
}

neffect_element::ptr _test_effect_particle_emitter()
{
	//-- 创建特效元素对象
	neffect_particle_emitter* elem=new neffect_particle_emitter();
	neffect_particle_emitter::ptr new_elem( elem );
	elem->init(_T("neffect_particle_emitter"));	
	elem->m_config.m_motion=ESFX_ExplodeMotion;
	elem->m_config.m_emit_speed=64;
	elem->m_config.m_damp=0.01f;
	elem->m_config.m_init_speed=vector3(0,0,500.0f);
	return new_elem;
}

void sfx_framework_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);

	neffect_element::ptr new_element;
	neffect_element::ptr new_element2;

	new_element = _test_effect_particle_emitter();
	new_element2 = _test_effect_quad();
	
	//-- create resource
	nresource_special_effect::ptr sfx_res = nresource_manager::instance()->new_special_effect(_T("TetsSfxRes"));
	sfx_res->add_element( new_element );
	sfx_res->add_element( new_element2 );
	sfx_res->post_edit_change(true);

#if 0 // 顺便测试一下SFX资源序列化
	resource_location test_loc(_T("demo_data:terrain/test.sfx"));
	sfx_res->save_to_file(test_loc, true);

	nresource_special_effect::ptr test_sfx_load = nresource_manager::instance()->load_special_effect(test_loc);
#endif

	//-- create instance
	m_actor = m_level_ptr->create_actor(_T("SFXTestActor"));
	nspecial_effect_instance::ptr sfx_inst = m_actor->create_component_t<nspecial_effect_instance>(_T("SFXTestInst"));
	sfx_inst->reset_resource( sfx_res );
	m_actor->move(vector3(0,100.0f,0),vector3::zero,vector3(1.0f,1.0f,1.0f));
	sfx_inst->play();

	cam->set_look_at(vector3::zero);
}

void sfx_framework_test::tick(float run_time)
{
	static float last_time=0;
	if(run_time-last_time>0.5f)
	{
		if(m_actor!=NULL)
		{
			m_actor->move(vector3(rand()%1000-500.0f,rand()%1000-500.0f,rand()%1000-500.0f),vector3::zero,vector3(1.0f,1.0f,1.0f));
		}
		last_time=run_time;
	}
	
}