#ifndef _NEXUS_EDITOR_CAMERA_CONTROLLER_H_
#define _NEXUS_EDITOR_CAMERA_CONTROLLER_H_
#include "ncore.h"
#include "camera_controller.h"

namespace nexus
{
	class nAPI editor_camera_controller :
		public camera_controller
	{
	public:
		editor_camera_controller(void);
		virtual ~editor_camera_controller(void);

		virtual void update_camera(ncamera* cam)
		{
			m_compute.update_camera_view(cam);
		}

		void set_look_at(const vector3& look_at)
		{
			m_compute.set_look_at(look_at);
		}

		void on_mouse_left_down(const npoint& pt);
		void on_mouse_left_up();
		void on_mouse_right_down();
		void on_mouse_right_up();
		void on_mouse_wheel(int delta);
		void on_mouse_move(const npoint& pt, bool ctrl_down);

		void set_factor(float move, float rotate, float wheel)
		{
			m_move_factor = move;
			m_rotate_factor = rotate;
			m_wheel_factor = wheel;
		}

	private:
		camera_YPR	m_compute;
		npoint		m_last_drag_pt;
		bool		m_left_down;
		bool		m_right_down;
		
		float		m_move_factor,
					m_rotate_factor,
					m_wheel_factor;
	};
}//namespace nexus

#endif //_NEXUS_EDITOR_CAMERA_CONTROLLER_H_