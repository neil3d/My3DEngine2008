#pragma once
#include "ntest_case.h"

class basic_anim_mesh_test :
	public nrender_test
{
public:
	basic_anim_mesh_test(void);
	virtual ~basic_anim_mesh_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(basic_anim_mesh_test)
};
