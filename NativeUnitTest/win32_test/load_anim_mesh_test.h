#pragma once
#include "ntest_case.h"

class load_anim_mesh_test :
	public nresource_test
{
public:
	load_anim_mesh_test(void);
	virtual ~load_anim_mesh_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(load_anim_mesh_test)
};
