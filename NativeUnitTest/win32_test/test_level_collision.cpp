#include "StdAfx.h"
#include "test_level_collision.h"
#include "../../NexusNative/nengine/physics/nphys_debug_render.h"
#include "../../NexusNative/nengine/physics/nphysics_framework.h"
#include <sstream>

nDEFINE_CLASS(test_level_collision, nphys_test)


extern nactor::ptr _load_anim_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds);
extern nactor::ptr _load_skel_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds);


test_level_collision::test_level_collision(void)
{
	m_phys_debug_render=false;
	m_add_rigid_body=false;
}

test_level_collision::~test_level_collision(void)
{
}

void test_level_collision::open(perspective_camera_controller* cam)
{
	resource_location loc(_T("game_data"), _T("level"));
	m_level_ptr = nengine::instance()->load_level(loc);

	vector3 dir(50, 50,50);
	add_direct_light(vec_normalize(dir));

	// begin play
	nengine::instance()->begin_play();

	nphys_scene* scene = m_level_ptr->get_phys_scene();
	if( !scene )
	{
		return;
	}
	// cache gravity
 	scene->get_gravity( m_level_gravity ); 

	const vector3 start( 50.f, 2000.f, 50.f );
	const vector3 end( 0.f, -200.f, 0.f );
	nphys_async_raycast_result result;
	scene->closest_raycast(result,start,end,false);
	if( !result.bhit )
	{
		return;
	}

	vector3 start_ctrl_pos = result.get_hit_location(); 


	// 创建phys controller
	for( int index=0; index<TEST_CONTROLLER_NUM; index++)
	{
		// 创建一个角色
		box_sphere_bounds bounds;
		nactor::ptr arthas = _load_skel_Arthas(m_level_ptr, bounds);
		
		// 创建角色控制器
		nphys_character_controller::ptr controller= boost::shared_static_cast<nphys_character_controller>(arthas->create_controller( _T("nphys_character_controller") ));
		nASSERT( controller );
		m_phys_controller.push_back(controller);

		// 初始化PhysX Controller

		controller->init_character_controller( scene->get_phys_engine()->get_nx_ccm(), scene->get_nx_scene(), start_ctrl_pos,30.f,100.f );
	}
}

void test_level_collision::tick(float run_time)
{
	nphys_test::tick(run_time);
	
	if( m_level_ptr.get() == NULL )
	{
		return;
	}

	nphys_scene* phys_scene = m_level_ptr->get_phys_scene();
	if( phys_scene && phys_scene->get_nx_scene() )
	{
		const vector3 start( 50.f, 2000.f, 50.f );
		for( int i=0; i<TEST_RAY_NUM; i++ )
		{
			const vector3 end( i*30.f, -200.f, i*50.f );
			//phys_scene->async_raycast( start, end, true, &m_raycast_ret[i] );
			phys_scene->closest_raycast(m_raycast_ret[i],start,end,true);
		}
	}

	// test move controller
	test_move_controller(run_time);
}

void test_level_collision::test_move_controller( float delta_time )
{
	for ( phys_controller_set::iterator ctrl_it = m_phys_controller.begin();
		ctrl_it!=m_phys_controller.end(); ctrl_it++ )
	{
		nphys_character_controller::ptr controller = *ctrl_it;
		if( controller.get() )
		{
			controller->move( vector3(0.f,m_level_gravity.y,1.0f), 550.f,delta_time );
		}
	}
}

void test_level_collision::debug_draw(nrender_primitive_draw_interface* PDI)
{
	if( m_level_ptr.get() == NULL )
	{
		return;
	}
	
	nphys_scene* phys_scene = m_level_ptr->get_phys_scene();
	if( phys_scene && phys_scene->get_nx_scene() )
	{
		// draw debug information
		if ( m_phys_debug_render )
		{
			physx_render_data( phys_scene->get_nx_scene()->getDebugRenderable(), PDI );
		}

		// test raycast
		std::wostringstream ss;
		ss << _T("mouse point: X=") << m_mouse_pt.x
			<< _T(", Y=") << m_mouse_pt.y
			;
		PDI->draw_debug_string(2, 18, ss.str(), color4ub(5,255,255,255));

		const vector3 start( 50.f, 2000.f, 50.f );
		for( int i=0; i<TEST_RAY_NUM; i++ )
		{
			const vector3 end( i*30.f, -200.f, i*50.f );
			PDI->draw_line( start, end, color4f(1.f, 1.f, 0.f, 1.f) );

			if( m_raycast_ret[i].bhit )
			{
				PDI->draw_wire_star(m_raycast_ret[i].get_hit_location(), 20.f, color4f(1.f, 0.f, 0.f, 1.f) );
			}
		}

		const vector3 ray_s =  vector3(500.f,600.f,500.f);
		const vector3 ray_e =  vector3(-500.f,-200.f,0.f);
		PDI->draw_line( ray_s, ray_e, color4f(1.f, 0.f, 1.f, 1.f) );

		nphys_async_raycast_result ret;
		if( phys_scene->closest_raycast(ret, ray_s, ray_e,true) )
		{
			PDI->draw_wire_star(ret.get_hit_location(), 20.f,  color4f(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void test_level_collision::on_mouse_move( const npoint& pt )
{
	m_mouse_pt = pt;
}

extern nresource_static_mesh::ptr _load_man_mesh();
void test_level_collision::add_rigid_body()
{
	const vector3 start( 50.f, 2000.f, 50.f );
	if( m_add_rigid_body )
	{
		// 重置位置
		nstatic_mesh_component::ptr collision_cmp = boost::shared_dynamic_cast<nstatic_mesh_component>(m_test_rigid_body->get_collision_cmp());
		if( collision_cmp.get() )
		{
			collision_cmp->set_nx_position(start);
		}

		return;
	}

	// 创建测试StaticMesh
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();
	box_sphere_bounds bounds = mesh_ptr->get_bounds();

	m_test_rigid_body = m_level_ptr->create_actor(_T("man_mesh_actor"));

	shared_ptr<nstatic_mesh_component> comp_ptr 
		= m_test_rigid_body->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);
	//mesh_ptr->generate_simple_mesh();

	// 创建物理
	mesh_ptr->set_pretriangle_collision(true);
	comp_ptr->m_disable_all_rigidbody = false;
	m_test_rigid_body->set_static(false);
	m_test_rigid_body->set_kinematic(false);
	m_test_rigid_body->set_collision_cmp( boost::shared_dynamic_cast<nactor_component>(comp_ptr) );
	m_test_rigid_body->set_physics_state(Phys_RigidBody);

	m_test_rigid_body->set_location(start);
	m_test_rigid_body->init_phys();

	m_add_rigid_body = true;
}