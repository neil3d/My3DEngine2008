#pragma once
#include "ntest_case.h"

class test_mtl_alpha_mask :
	public nrender_test
{
public:
	test_mtl_alpha_mask(void);
	virtual ~test_mtl_alpha_mask(void);

	virtual void open(editor_camera_controller* cam);	

	nDECLARE_CLASS(test_mtl_alpha_mask)
};
