#pragma once
#include "ntest_case.h"

class shadow_test :
	public nrender_test
{
public:
	shadow_test(void);
	virtual ~shadow_test(void);

	virtual void open(perspective_camera_controller* cam);
	virtual void tick(float run_time);

private:
	nactor::ptr	m_man;
	
	nDECLARE_CLASS(shadow_test)
};
