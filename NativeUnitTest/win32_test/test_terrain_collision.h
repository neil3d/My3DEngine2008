#pragma once
#include "ntest_case.h"

class test_terrain_collision :
	public nphys_test
{
public:
	test_terrain_collision(void);
	virtual ~test_terrain_collision(void);

	virtual void open(perspective_camera_controller* cam);	

private:
	nDECLARE_CLASS(test_terrain_collision)
};
