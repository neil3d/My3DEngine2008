#include "StdAfx.h"
#include "editor_camera_controller.h"

namespace nexus
{
	editor_camera_controller::editor_camera_controller(void)
		: m_left_down(0), m_right_down(0),
		m_move_factor(4), m_rotate_factor(0.005f), m_wheel_factor(0.2f)
	{
	}

	editor_camera_controller::~editor_camera_controller(void)
	{
	}

	void editor_camera_controller::on_mouse_left_down(const npoint& pt)
	{
		m_left_down = true;
		m_last_drag_pt = pt;
	}

	void editor_camera_controller::on_mouse_left_up()
	{
		m_left_down = false;
	}

	void editor_camera_controller::on_mouse_right_down()
	{
		m_right_down = true;
	}

	void editor_camera_controller::on_mouse_right_up()
	{
		m_right_down = false;
	}

	void editor_camera_controller::on_mouse_wheel(int delta)
	{
		// 处理鼠标滚轮
		m_compute.move_dist(delta*m_wheel_factor);
	}

	void editor_camera_controller::on_mouse_move(const npoint& pt, bool ctrl_down)
	{
		if( m_left_down )
		{
			npoint d = pt-m_last_drag_pt;

			if( m_right_down)	// 两键一起拖动
			{		
				vector3 delta(0,0,0);

				if( ctrl_down ) //  ctrl
					delta += m_compute.m_forward*d.y * m_move_factor;
				else
					delta += m_compute.m_up*d.y * m_move_factor;

				delta -= m_compute.m_side*d.x * m_move_factor;
				
				m_compute.move_look_at(delta);
			}
			else
			{
				m_compute.move_yaw(d.x * m_rotate_factor);
				m_compute.move_pitch(-d.y * m_rotate_factor);
			}
		}

		m_last_drag_pt = pt;
	}
}//namespace nexus