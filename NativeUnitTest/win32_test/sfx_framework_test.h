#pragma once
#include "ntest_case.h"

/**
 *	测试特效资源-特效元素-特效实例等几个框架类的流程
*/
class sfx_framework_test :
	public nrender_test
{
public:
	nactor::ptr m_actor;

	sfx_framework_test(void);
	virtual ~sfx_framework_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void tick(float run_time);

	nDECLARE_CLASS(sfx_framework_test)
};
