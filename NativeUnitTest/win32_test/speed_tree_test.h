#pragma once
#include "ntest_case.h"

class speed_tree_test :
	public nrender_test
{
public:
	speed_tree_test(void);
	virtual ~speed_tree_test(void);

	virtual void open(perspective_camera_controller* cam);	

	nDECLARE_CLASS(speed_tree_test)
};
