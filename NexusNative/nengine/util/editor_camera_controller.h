#ifndef _NEXUS_EDITOR_CAMERA_CONTROLLER_H_
#define _NEXUS_EDITOR_CAMERA_CONTROLLER_H_
#include "ncore.h"
#include "camera_controller.h"

namespace nexus
{
	class nAPI perspective_camera_controller :
		public camera_controller
	{
	public:
		perspective_camera_controller(void);
		virtual ~perspective_camera_controller(void);

		virtual void update_camera(ncamera* cam)
		{
			m_compute.update_camera_view(cam);
		}

		void set_look_at(const vector3& look_at)
		{
			m_compute.set_look_at(look_at);
		}

		void move_yaw(float y)	{	m_compute.move_yaw(y);}
		void move_pitch(float p)	{	m_compute.move_pitch(p);}	

		void on_mouse_wheel(int delta);
		void on_mouse_move(const npoint& pt, bool ctrl_down);

		virtual void zoom_extents(const AABBox& box, const ncamera* cam)
		{
			m_compute.zoom_extents(box, cam);
		}

	protected:
		camera_YPR	m_compute;		
	};

	class nAPI ortho_camera_controller:
		public camera_controller
	{
	public:
		ortho_camera_controller(void);
		virtual ~ortho_camera_controller(void)	{}

		virtual void update_camera(ncamera* cam);
		virtual void on_mouse_wheel(int delta);
		virtual void on_mouse_move(const npoint& pt, bool ctrl_down);

		void set_orientation(TCHAR axis);

	private:
		vector3 get_forward() const;
		vector3 get_side() const;
		vector3 get_up() const;
	private:
		vector3 m_look_at;		
		float	m_dist;
		TCHAR	m_axis;
		float	m_view_size;
	};
}//namespace nexus

#endif //_NEXUS_EDITOR_CAMERA_CONTROLLER_H_