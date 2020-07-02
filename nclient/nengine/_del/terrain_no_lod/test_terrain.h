#pragma once
#include "ntest_case.h"

class test_terrain :
	public nrender_test
{
public:
	test_terrain(void);
	virtual ~test_terrain(void);

	virtual void open(editor_camera_controller* cam);	

private:		
	nDECLARE_CLASS(test_terrain)
};
