#include "StdAfx.h"
#include "test_staticmesh_collision.h"


nDEFINE_CLASS(test_staticmesh_collision, nphys_test)

test_staticmesh_collision::test_staticmesh_collision(void)
{
}

test_staticmesh_collision::~test_staticmesh_collision(void)
{
}


extern nresource_static_mesh::ptr _load_man_mesh();
void test_staticmesh_collision::open(perspective_camera_controller* cam)
{
	nphys_test::open(cam);

	add_point_light();
	// 创建测试StaticMesh
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();
	box_sphere_bounds bounds = mesh_ptr->get_bounds();
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("man_mesh_actor"));

	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);

	cam->set_look_at(bounds.origin);
	cam->on_mouse_wheel( bounds.sphere_radius*2.f );

	m_light_anim_radius = bounds.sphere_radius*1.5f;
	//mesh_ptr->generate_simple_mesh();
	m_staticmesh_actor = actor_ptr;
	m_comp_ptr = comp_ptr;

	// 创建物理
	mesh_ptr->set_pretriangle_collision(true);
	m_comp_ptr->m_disable_all_rigidbody = false;
	
	nengine::instance()->begin_play();
}

void test_staticmesh_collision::close()
{
	// do gc
	m_comp_ptr.reset();

	nphys_test::close();
}

void test_staticmesh_collision::tick(float run_time)
{
	nphys_test::tick( run_time );
}

void test_staticmesh_collision::debug_draw(nrender_primitive_draw_interface* PDI)
{
	//m_comp_ptr->draw_simple_mesh(PDI, color4f(1,1,0,1));
}