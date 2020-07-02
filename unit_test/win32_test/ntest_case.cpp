#include "StdAfx.h"
#include "ntest_case.h"

nDEFINE_VIRTUAL_CLASS(ntest_case, nobject)
nDEFINE_VIRTUAL_CLASS(nrender_test, ntest_case)
nDEFINE_VIRTUAL_CLASS(nphys_test, ntest_case)

void ntest_case::add_point_light()
{
	wostringstream oss;
	oss << _T("light_actor_")
		<< ::rand();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());	
	new_actor->set_static(false);

	// 使用一个shape component显示light的位置
	shared_ptr<nshape_component> box(
		new_actor->create_component_t<nshape_component>(_T("box"))
		);	
	box->create_box(10,10,10);		

	// 创建light component
	shared_ptr<nlight_component> lgt(
		new_actor->create_component_t<nlight_component>(_T("point_light"))
		);	
	lgt->create_point_light();
	npoint_light_proxy* lgt_proxy = lgt->get_point_light();
	lgt_proxy->m_diffuse_color = color4f(1, 1, 1, 1);
	lgt_proxy->m_position = vector3(0, m_light_anim_radius, 0);	// actor space position
	lgt_proxy->m_attenuation = vector3(0.5f, 0.0001f,0);

	m_light_array.push_back(new_actor);
}

void ntest_case::anim_lights(float run_time)
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
		switch(i%3)
		{
		case 0:
			pos = vector3(sinf(t)*r, cosf(t)*r, sinf(t)*cosf(t)*r);
			break;
		case 1:
			pos = vector3(sinf(t)*r, cosf(t)*r, 0);
			break;
		case 2:
			pos = vector3(sinf(t)*r, 0, cosf(t)*r);
			break;
		}

		matrix44 mat;
		mat_set_translation(mat,pos.x, pos.y, pos.z);
		lgt_actor->move(mat);
	}
}

void ntest_case::add_direct_light(const vector3& direction)
{
	wostringstream oss;
	oss << _T("light_actor_")
		<< ::rand();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());		
	new_actor->set_static(false);

	//-- 创建light component
	shared_ptr<nlight_component> lgt(
		new_actor->create_component_t<nlight_component>(_T("direct_light"))
		);	
	lgt->create_directional_light();
	ndirectional_light_proxy* dir_lgt = lgt->get_directional_light();

	dir_lgt->m_diffuse_color = color4f(0.7f, 0.7f, 0.7f, 1);
	dir_lgt->m_ambient_color = color4f(0.3f, 0.3f, 0.3f, 0.0f);
	dir_lgt->m_direction = direction;

	//--
	m_light_array.push_back(new_actor);
}

void ntest_case::remove_last_light()
{
	if( !m_light_array.empty() )
	{
		nactor::ptr rm_actor = m_light_array[m_light_array.size()-1];
		m_light_array.pop_back();
		m_level_ptr->free_actor(rm_actor->get_name());
	}
}