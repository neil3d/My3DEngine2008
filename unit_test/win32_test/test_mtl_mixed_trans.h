#pragma once
#include "ntest_case.h"

class test_mtl_mixed_trans :
	public nrender_test
{
public:
	test_mtl_mixed_trans(void);
	virtual ~test_mtl_mixed_trans(void);

	virtual void open(editor_camera_controller* cam);	

	nDECLARE_CLASS(test_mtl_mixed_trans)
};
