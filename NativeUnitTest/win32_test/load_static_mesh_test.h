#pragma once
#include "ntest_case.h"

class load_static_mesh_test :
	public nresource_test
{
public:
	load_static_mesh_test(void);
	virtual ~load_static_mesh_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(load_static_mesh_test)
};
