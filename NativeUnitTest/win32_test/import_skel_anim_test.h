#pragma once
#include "ntest_case.h"

class import_skel_anim_test :
	public nresource_test
{
public:
	import_skel_anim_test(void);
	virtual ~import_skel_anim_test(void);

	virtual void open(perspective_camera_controller* cam);
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);

private:
	nactor::ptr						m_skeletal_mesh_actor;
	nskeletal_mesh_component::ptr	m_skeletal_mesh_component;

	nDECLARE_CLASS(import_skel_anim_test)
};
