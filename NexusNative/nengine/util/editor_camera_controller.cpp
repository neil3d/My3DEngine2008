#include "StdAfx.h"
#include "editor_camera_controller.h"
#include <Windows.h>
namespace nexus
{
	perspective_camera_controller::perspective_camera_controller(void)		
	{
		m_compute.move_yaw(nDegToRad(45));
		m_compute.move_pitch(nDegToRad(-45));
	}

	perspective_camera_controller::~perspective_camera_controller(void)
	{
	}

	void perspective_camera_controller::on_mouse_wheel(int delta)
	{
		// 处理鼠标滚轮
		if(!m_right_down)
			m_compute.move_dist(delta*m_wheel_factor);
	}

	void perspective_camera_controller::on_mouse_move(const npoint& pt, bool ctrl_down)
	{
		if(m_middle_down)
		{
			npoint d = pt-m_middle_drag_pt;
			vector3 delta(0,0,0);

			if( ctrl_down ) //  ctrl
				delta += m_compute.m_forward*d.y * m_move_factor;
			else
				delta += m_compute.m_up*d.y * m_move_factor;

			delta += m_compute.m_side*d.x * m_move_factor;

			m_compute.move_look_at(delta);

			m_middle_drag_pt=pt;
		}
		if(m_left_down )	// 左键拖动
		{		
			vector3 delta(0,0,0);
			npoint d = pt-m_left_drag_pt;

			if( ctrl_down ) //  ctrl
				delta += m_compute.m_forward*d.y * m_move_factor;
			else
				delta += m_compute.m_up*d.y * m_move_factor;

			delta += m_compute.m_side*d.x * m_move_factor;
			
			m_compute.move_look_at(delta);

			m_left_drag_pt = pt;
			m_right_drag_pt = pt;

			// 左键优先
			m_right_down = false;
		}
		else if(m_right_down)
		{
			npoint d = pt-m_right_drag_pt;
			m_compute.move_yaw(-d.x * m_rotate_factor);
			m_compute.move_pitch(-d.y * m_rotate_factor);

			m_left_drag_pt = pt;
			m_right_drag_pt = pt;
		}
	}

	bool perspective_camera_controller::on_key_down( unsigned int key )
	{
		vector3 delta(0,0,0);
		switch (key)
		{
		case VK_UP:
			{
				delta += m_compute.m_forward*100 * m_move_factor;
				m_compute.move_look_at(delta);
			}
			break;
		case VK_DOWN:
			{
				delta -= m_compute.m_forward*100 * m_move_factor;
				m_compute.move_look_at(delta);
				break;
			}
		case VK_LEFT:
			{				
				delta += m_compute.m_side*100 * m_move_factor;
				m_compute.move_look_at(delta);
			}
			break;
		case VK_RIGHT:
			{
				delta -= m_compute.m_side*100 * m_move_factor;
				m_compute.move_look_at(delta);
			}
			break;
		case VK_PRIOR:
			{
				delta += m_compute.m_up*100 * m_move_factor;
				m_compute.move_look_at(delta);
			}
			break;
		case VK_NEXT:
			{
				delta -= m_compute.m_up*100 * m_move_factor;
				m_compute.move_look_at(delta);
			}
			break;

		default:
			return camera_controller::on_key_down(key);
		}

		return true;
	}

	ortho_camera_controller::ortho_camera_controller(void):m_dist(2000),m_look_at(0,0,0)
	{
		m_axis = _T('Y');

		m_view_size = 4000;
	}

	void ortho_camera_controller::update_camera(ncamera* cam)
	{
		float view_w = m_view_size;
		float view_h = m_view_size/cam->get_viewport_aspect();

		vector3 eye = m_look_at-get_forward()*m_dist;

		cam->set_lookat(eye, m_look_at, get_up());
		cam->set_ortho(view_w, view_h, cam->get_znear(), cam->get_zfar());
	}

	void ortho_camera_controller::on_mouse_wheel(int delta)
	{
		if(!m_right_down)
			m_view_size += delta*m_wheel_factor*10;
		if(!m_right_down)
		{
			m_dist+=delta*m_wheel_factor;
			if(m_dist<1.0f) m_dist=1.0f;
		}
	}

	void ortho_camera_controller::on_mouse_move(const npoint& pt, bool ctrl_down)
	{
		(void)ctrl_down;

		if(m_middle_down)
		{
			npoint d = pt-m_middle_drag_pt;

			vector3 offset_x = get_side()*(d.x*m_move_factor);
			vector3 offset_y = get_up()*(d.y*m_move_factor);

			m_look_at -= offset_x;
			m_look_at += offset_y;

			m_middle_drag_pt=pt;
		}
	}

	void ortho_camera_controller::set_orientation(TCHAR axis)
	{
		m_axis = axis;
	}

	vector3 ortho_camera_controller::get_forward() const
	{
		switch(m_axis)
		{
		case _T('X'):
			return vector3(-1, 0, 0);
			break;
		case _T('Y'):
			return vector3(0, -1, 0);
			break;
		case _T('Z'):
			return vector3(0, 0, 1);
			break;
		}

		return vector3(0, 1, 0);;
	}

	vector3 ortho_camera_controller::get_side() const
	{
		switch(m_axis)
		{
		case _T('X'):
			return vector3(0, 0, 1);
			break;
		case _T('Y'):
			return vector3(1, 0, 0);
			break;
		case _T('Z'):
			return vector3(1, 0, 0);
			break;
		}

		return vector3(0, 1, 0);;
	}

	vector3 ortho_camera_controller::get_up() const
	{
		vector3 forward = get_forward();
		vector3 side = get_side();
		return vec_cross(forward, side);
	}
}//namespace nexus