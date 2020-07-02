#pragma once
#include "ntest_case.h"

class load_skeletal_mesh_animset_test :
	public nresource_test
{
public:
	load_skeletal_mesh_animset_test(void);
	virtual ~load_skeletal_mesh_animset_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(load_skeletal_mesh_animset_test)
};
