#pragma once
#include "ntest_case.h"

class speed_tree_test :
	public nresource_test
{
public:
	speed_tree_test(void);
	virtual ~speed_tree_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void close();

private:
	nspeed_tree_component::ptr m_spt_comp;

	nDECLARE_CLASS(speed_tree_test)
};
