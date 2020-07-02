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
		//m_sky.reset();
		nrender_test::close();
	}

	virtual void tick(float run_time,float delta_time);
	virtual void debug_draw(nrender_primitive_draw_interface* PDI);

	virtual void anim_lights(float run_time)
	{
		if( m_light_array.empty() )
			return;

		float step = 20;
		for(size_t i=0; i<m_light_array.size(); i++)
		{
			nactor::ptr lgt_actor = m_light_array[i];

			float t = run_time*0.6f+step*i;
			float r = m_light_anim_radius;
			vector3 pos;

			pos = vector3(sinf(t)*r, r/4, cosf(t)*r);
			//pos = vector3(0, m_light_anim_radius, 0);
			lgt_actor->move(pos, vector3::zero, vector3::one);

			lgt_actor->look_at(vector3::zero,vector3(1,0,0));
		}
	}
	
private:
	//ndynamic_sky_dome::ptr		m_sky;
	ndirectional_light_component*	m_lgt;
	float			m_theta,
					m_phi,
					m_turbidity,
					m_overcast,
					m_expose,
					m_gama;

	nDECLARE_CLASS(sky_dome_test)
};
