#pragma once
#include "ntest_case.h"
#include "../../NexusNative/nengine/physics/nphys_util.h"
#include "../../NexusNative/nengine/physics/nphys_scene_query_report.h"
#include "../../NexusNative/nengine/physics/nphys_character_controller.h"

class test_level_collision :
	public nphys_test
{
public:
	test_level_collision(void);
	virtual ~test_level_collision(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);

	virtual void tick(float run_time);

	virtual void on_mouse_move( const npoint& pt );
	virtual void add_rigid_body();

private:
	virtual void test_move_controller( float delta_time );

private:
	bool			m_phys_debug_render;
	npoint			m_mouse_pt;

	vector3			m_level_gravity;

	enum { TEST_RAY_NUM = 100 };
	nphys_async_raycast_result m_raycast_ret[TEST_RAY_NUM];

	// 测试PhysX character controller
	enum { TEST_CONTROLLER_NUM = 1 };
	typedef std::vector<nphys_character_controller::ptr> phys_controller_set; 
	phys_controller_set		m_phys_controller;

	// 测试动态创建刚体
	nactor::ptr				m_test_rigid_body;
	bool					m_add_rigid_body;

	nDECLARE_CLASS(test_level_collision)
};
