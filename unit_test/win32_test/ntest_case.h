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

	ntest_case(void):m_light_anim_radius(2000)
	{}
	virtual ~ntest_case(void)	{}

	virtual void open(editor_camera_controller* cam)	
	{
		//-- init level
		m_level_ptr = nengine::instance()->create_level(_T("test_lv"), _T("nlevel"));
		m_level_ptr->init(_T("nplain_scene"));
	}
	
	virtual void close() 
	{
		m_light_array.clear();
		if( m_level_ptr )
			nengine::instance()->free_level(m_level_ptr);
		m_level_ptr.reset();		
	}

	virtual void tick(float run_time)
	{
		anim_lights(run_time);
	}

	virtual void debug_draw(nrender_primitive_draw_interface* PDI)	{}
	void add_point_light();
	void add_direct_light(const vector3& direction);
	void remove_last_light();
protected:
	void anim_lights(float run_time);

	nlevel::ptr					m_level_ptr;
	
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

	nDECLARE_VIRTUAL_CLASS(nrender_test)
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