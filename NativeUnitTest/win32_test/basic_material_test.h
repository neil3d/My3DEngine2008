#pragma once
#include "ntest_case.h"

class basic_material_test : public nrender_test
{
public:
	basic_material_test(void);
	~basic_material_test(void);

	virtual void open(perspective_camera_controller* cam);	

protected:
	virtual void anim_lights(float run_time)
	{
		if( m_light_array.empty() )
			return;

		float step = 20;
		for(size_t i=0; i<m_light_array.size(); i++)
		{
			nactor::ptr lgt_actor = m_light_array[i];

			lgt_actor->move(vector3(0,m_light_anim_radius,0), vector3::zero, vector3::one);
		}
	};
private:
	void test_material_static();
	nmtl_static::ptr create_material(const nstring& material_name);

	nDECLARE_CLASS(basic_material_test)
};
