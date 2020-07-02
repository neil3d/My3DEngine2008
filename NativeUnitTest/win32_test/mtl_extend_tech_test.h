#pragma once
#include "ntest_case.h"

class mtl_extend_tech_test :
	public nrender_test
{
public:
	mtl_extend_tech_test(void);
	virtual ~mtl_extend_tech_test(void);

	virtual void open(perspective_camera_controller* cam);	

private:
	nDECLARE_CLASS(mtl_extend_tech_test)
};
