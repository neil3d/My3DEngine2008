#pragma once
#include "ntest_case.h"

class test_staticmesh_collision :
	public nphys_test
{
public:
	test_staticmesh_collision(void);
	virtual ~test_staticmesh_collision(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void close();

	virtual void tick(float run_time);

protected:
	nactor::ptr							m_staticmesh_actor;
	shared_ptr<nstatic_mesh_component>	m_comp_ptr;

private:
	nDECLARE_CLASS(test_staticmesh_collision)
};
