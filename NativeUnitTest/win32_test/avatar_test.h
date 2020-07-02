#pragma once
#include "ntest_case.h"

class avatar_test :
	public nrender_test
{
public:
	avatar_test(void);
	virtual ~avatar_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(avatar_test)
};
