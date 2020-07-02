#pragma once
#include "ntest_case.h"

class sky_dome_test : public nrender_test
{
public:
	sky_dome_test(void);
	virtual ~sky_dome_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void close()
	{
		m_sky.reset();
		nrender_test::close();
	}

	virtual void tick(float run_time);
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	
private:
	ndynamic_sky_dome::ptr		m_sky;
	ndirectional_light_proxy*	m_lgt;
	float			m_theta,
					m_phi,
					m_turbidity,
					m_overcast,
					m_expose,
					m_gama;

	nDECLARE_CLASS(sky_dome_test)
};
