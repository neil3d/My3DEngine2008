#include "StdAfx.h"
#include <ostream>
#include "player_ctrl.h"

namespace nexus
{
	const float PLAYER_RUN_SPEED = 200;

	nDEFINE_CLASS(player_ctrl, nactor_controller)

		player_ctrl::player_ctrl(void)
	{
	}

	player_ctrl::~player_ctrl(void)
	{
	}

	void player_ctrl::_init(nactor* actor)
	{
		nactor_controller::_init(actor);

		nactor_component::ptr comp = actor->get_component(_T("anim_mesh"));
		m_anim_comp = boost::dynamic_pointer_cast<nanim_mesh_component>(comp);
	}

	void player_ctrl::process_input_message(const MSG& msg)
	{	
		switch( msg.message )
		{
		case WM_KEYDOWN:			
			on_key_down(msg.wParam);			
			break;
		case WM_KEYUP:
			on_key_up(msg.wParam);			
			break;
		}
	}

	void player_ctrl::update_actor(float delta_time)
	{
		object_space os = m_actor->get_space();	

		bool runing = false;
		for(int i=0; i<4; i++)
		{
			os.location += m_vec[i]*delta_time;		

			float speed = vec_length(m_vec[i]);

			runing |= fabs(speed-PLAYER_RUN_SPEED)<0.1f;
		}
		/*ncheck_result chk;
		vector3 chk_len(0,2000,0);
		if(m_actor->get_owner()->line_check(chk, os.location+chk_len, os.location-chk_len))
			os.location = chk.location+vector3(0,20,0);*/

		m_actor->move(os.location, os.rotation, os.scale);

		if(runing)
			m_anim_comp->play_anim(_T("run"), true, -1);
		else
			m_anim_comp->play_anim(_T("std"), true, -1);
	}

	void player_ctrl::on_key_down(unsigned int vk)
	{
		object_space os = m_actor->get_space();	

		// todo : matrix44::get_axis_x()²»ÕýÈ·
		vector3 axis_z(0,0,1);
		vector3 axis_x(1,0,0);
		matrix44 rot_mat;
		mat_set_rotation_yaw_pitch_roll(rot_mat, os.rotation.y, os.rotation.x, os.rotation.z);

		axis_z = axis_z*rot_mat;
		axis_x = axis_x*rot_mat;

		switch(vk)
		{
		case 'W':
			m_vec[0] = axis_z*(-PLAYER_RUN_SPEED);
			break;
		case 'S':
			m_vec[1] = axis_z*PLAYER_RUN_SPEED;
			break;
		case 'A':
			m_vec[2] = axis_x*PLAYER_RUN_SPEED;
			break;
		case 'D':
			m_vec[3] = axis_x*(-PLAYER_RUN_SPEED);
			break;
		}
	}

	void player_ctrl::on_key_up(unsigned int vk)
	{
		switch(vk)
		{
		case 'W':
			m_vec[0] = vector3::zero;
			break;
		case 'S':
			m_vec[1] = vector3::zero;
			break;
		case 'A':
			m_vec[2] = vector3::zero;
			break;
		case 'D':
			m_vec[3] = vector3::zero;
			break;
		}
	}

	void player_ctrl::debug_draw(nrender_primitive_draw_interface* PDI)
	{
		object_space os = m_actor->get_space();	

		color4f line_color(1,1,1,1);
		
		PDI->begin_line_batch();
		for (int i=0; i<4; i++)
		{
			PDI->draw_line(os.location, os.location+m_vec[i], line_color);
		}		
		PDI->end_line_batch();
		
		color4ub txt_color(255,255,250,255);
		wostringstream oss;
		oss << _T("x = ") << os.location.x
			<< _T(",y = ") << os.location.y
			<< _T(",z = ") << os.location.z
			<< _T(",yaw = ") << os.rotation.y;
		PDI->draw_debug_string(2,2, oss.str(), txt_color);
	}
}//using namespace nexus