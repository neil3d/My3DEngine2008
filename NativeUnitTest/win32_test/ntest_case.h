/**
*	Nexus Engine - Win32 GUI测试程序
*
*	Author: Neil
*	Date:	Feb, 2009
*/


#pragma once
#include "ncore.h"
using namespace nexus;

/**
 *	测试用例的基类
*/
class ntest_case
	: public nobject
{
public:
	typedef boost::shared_ptr<ntest_case> ptr;

	ntest_case(void):m_light_anim_radius(1000)
	{}
	virtual ~ntest_case(void)	{}

	virtual void open(perspective_camera_controller* cam);
	
	virtual void close() 
	{
		m_teapot_ptr.reset();
		m_light_array.clear();
		if( m_level_ptr )
			nengine::instance()->destroy_level(m_level_ptr);
		m_level_ptr.reset();		
	}

	virtual void tick(float run_time,float delta_time)
	{
		anim_lights(run_time);
	}

	virtual void debug_draw(nrender_primitive_draw_interface* PDI)	
	{
		nactor::ptr na = m_level_ptr->first_actor();
		while( na )
		{
			na->draw_bounds(PDI,color4f(1,0,0,1));
			na = m_level_ptr->next_actor();
		}
	}

	virtual void debug_canvas(nui_canvas* canvas)	{}

	virtual void on_mouse_move( const npoint& pt ) {}
	virtual void on_key_down(unsigned int key) {}

	virtual void add_rigid_body() {}

	void add_point_light();
	void add_spot_light();
	void add_teapot(const nstring& actor_name,vector3 position,nmtl_base::ptr mtl_ptr);
	void add_sphere(const nstring& actor_name,vector3 position,nmtl_base::ptr mtl_ptr);
	ndirectional_light_component* add_direct_light(const vector3& direction);
	void remove_last_light();
	nlevel::ptr					m_level_ptr;
protected:
	virtual void anim_lights(float run_time);
	
	nresource_static_mesh::ptr	m_teapot_ptr;
	
	std::vector<nactor::ptr>	m_light_array;
	float						m_light_anim_radius;
private:

	nDECLARE_VIRTUAL_CLASS(ntest_case)
};

/**
 *	渲染相关测试的基类
 *	@使用这个派生类主要是为了对测试用例分组
*/
class nrender_test
	: public ntest_case
{
public:
	nrender_test(void)	{}
	virtual ~nrender_test(void)	{}

	nmtl_static::ptr create_test_material(const nstring& material_name);

	nDECLARE_VIRTUAL_CLASS(nrender_test)
};

/**
 *	资源导入导出等测试
*/
class nresource_test
	: public ntest_case
{
public:
	nresource_test(void):ntest_case(){}
	virtual ~nresource_test(void)	{}

	nDECLARE_VIRTUAL_CLASS(nresource_test)
};

/**
 *	物理相关测试的基类
*/
class nphys_test
	: public ntest_case
{
public:
	nphys_test(void)	{}
	virtual ~nphys_test(void)	{}

	nDECLARE_VIRTUAL_CLASS(nphys_test)
};