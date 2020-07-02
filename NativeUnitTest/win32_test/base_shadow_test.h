#pragma once
#include "ntest_case.h"

class base_shadow_test : public nrender_test
{
public:
	base_shadow_test(void);
	~base_shadow_test(void);

	virtual void open(perspective_camera_controller* cam);
	
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

			pos = vector3(sinf(t)*r, 100, cosf(t)*r);
			//pos = vector3(0, m_light_anim_radius, 0);
			lgt_actor->move(pos, vector3::zero, vector3::one);
		
			lgt_actor->look_at(vector3::zero,vector3(1,0,0));
		}
	}

private:
	nDECLARE_CLASS(base_shadow_test)
};
