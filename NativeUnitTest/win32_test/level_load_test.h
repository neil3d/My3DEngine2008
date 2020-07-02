#pragma once
#include "ntest_case.h"

class level_load_test :
	public nrender_test
{
public:
	level_load_test(void);
	virtual ~level_load_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);

	nDECLARE_CLASS(level_load_test)
};
