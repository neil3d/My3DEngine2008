#pragma once
#include "ntest_case.h"

class test_sample_mesh :
	public nphys_test
{
public:
	test_sample_mesh(void);
	virtual ~test_sample_mesh(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void close()
	{
		m_comp_ptr.reset();
		nphys_test::close();
	}
private:		
	shared_ptr<nstatic_mesh_component> m_comp_ptr;

	nDECLARE_CLASS(test_sample_mesh)
};
