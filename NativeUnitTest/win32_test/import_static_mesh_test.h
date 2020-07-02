#pragma once
#include "ntest_case.h"

class import_obj_test :
	public nresource_test
{
public:
	import_obj_test(void)	{}
	virtual ~import_obj_test(void)	{}

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(import_obj_test)
};

class import_fsm_test :
	public nresource_test
{
public:
	import_fsm_test(void)	{}
	virtual ~import_fsm_test(void)	{}

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(import_fsm_test)
};