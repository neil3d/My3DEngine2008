#pragma once
#include "ntest_case.h"

class basic_skel_mesh_test :
	public nrender_test
{
public:
	basic_skel_mesh_test(void);
	virtual ~basic_skel_mesh_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(basic_skel_mesh_test)
};