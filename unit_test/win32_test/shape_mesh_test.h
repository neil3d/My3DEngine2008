#pragma once
#include "ntest_case.h"

class shape_mesh_test :
	public nrender_test
{
public:
	shape_mesh_test(void);
	virtual ~shape_mesh_test(void);

	virtual void open(editor_camera_controller* cam);	

	nDECLARE_CLASS(shape_mesh_test)
};
