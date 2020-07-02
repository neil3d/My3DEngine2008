#include "StdAfx.h"
#include "player_camera_ctrl.h"

player_camera_ctrl::player_camera_ctrl(void)
{
}

player_camera_ctrl::~player_camera_ctrl(void)
{
}

void player_camera_ctrl::bind_player(nactor::ptr player)
{
	m_player = player;
	m_compute.set_look_at(player->get_space().location);
	m_compute.m_dist = 500;
	m_compute.m_pitch = nDegToRad(-45);	
}

void player_camera_ctrl::process_input_message(const MSG& msg)
{
	switch( msg.message )
	{	
	case WM_LBUTTONDOWN:
		{
			npoint pt(LOWORD(msg.lParam), HIWORD(msg.lParam));		
			on_mouse_left_down(pt);
		}
		break;
	case WM_LBUTTONUP:		
		on_mouse_left_up();
		break;
	case WM_RBUTTONDOWN:
		{
			npoint pt(LOWORD(msg.lParam), HIWORD(msg.lParam));		
			on_mouse_right_down(pt);
		}
		break;
	case WM_RBUTTONUP:		
		on_mouse_right_up();
		break;
	case WM_MOUSEMOVE:
		{
			npoint pt(LOWORD(msg.lParam), HIWORD(msg.lParam));
			bool ctrl = 0!=(msg.wParam&MK_CONTROL);
			on_mouse_move(pt, ctrl);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);		
			on_mouse_wheel(delta);
		}
		break;
	}// end of switch
}

void player_camera_ctrl::on_mouse_left_down(const npoint& pt)
{
	camera_controller::on_mouse_left_down(pt);
}

void player_camera_ctrl::on_mouse_left_up()
{
	camera_controller::on_mouse_left_up();
}

void player_camera_ctrl::on_mouse_right_down(const npoint& pt)
{
	camera_controller::on_mouse_right_down(pt);
}

void player_camera_ctrl::on_mouse_right_up()
{
	camera_controller::on_mouse_right_up();
}

void player_camera_ctrl::on_mouse_wheel(int delta)
{
	m_compute.move_dist(delta*m_wheel_factor);

	m_compute.m_dist = std::max(140.0f, m_compute.m_dist);
	m_compute.m_dist = std::min(4000.0f, m_compute.m_dist);
}

void player_camera_ctrl::on_mouse_move(const npoint& pt, bool ctrl_down)
{
	if(m_left_down)
		mouse_left_drag(pt, ctrl_down);
	else if(m_right_down)
		mouse_right_drag(pt, ctrl_down);
}

void player_camera_ctrl::mouse_left_drag(const npoint& pt, bool ctrl_down)
{
	npoint d = pt-m_left_drag_pt;
	
	m_compute.move_yaw(d.x * m_rotate_factor);
	m_compute.move_pitch(-d.y * m_rotate_factor);

	m_left_drag_pt = pt;
}

void player_camera_ctrl::mouse_right_drag(const npoint& pt, bool ctrl_down)
{
	npoint d = pt-m_right_drag_pt;
	m_right_drag_pt = pt;

	//-- rotate camera
	m_compute.move_yaw(d.x * m_rotate_factor);
	m_compute.move_pitch(-d.y * m_rotate_factor);
	
	//-- rotate player
	object_space os = m_player->get_space();	
	//os.rotation.x = m_compute.m_pitch;	
	os.rotation.y = -m_compute.m_yaw;	
	//os.rotation.z = m_compute.m_roll;	

	m_player->move(os.location, os.rotation, os.scale);
}

void player_camera_ctrl::lock_player_pos()
{
	vector3 pos = m_player->get_space().location;
	pos.y += 140;
	m_compute.set_look_at(pos);
}