#ifndef _NEXUS_CAMERA_CONTROLLER_H_
#define _NEXUS_CAMERA_CONTROLLER_H_
#include "ncore.h"

namespace nexus
{
	/**
	 * 摄像机控制基类
	*/
	class nAPI camera_controller
	{
	public:
		camera_controller(void);
		virtual ~camera_controller(void)	{	}

		virtual void update_camera(ncamera* cam) = 0;

		virtual void on_mouse_left_down(const npoint& pt);
		virtual void on_mouse_left_up();
		virtual void on_mouse_right_down(const npoint& pt);
		virtual void on_mouse_right_up();
		virtual void on_mouse_middle_down(const npoint& pt);
		virtual void on_mouse_middle_up();
		virtual void on_mouse_wheel(int delta) = 0;
		virtual bool on_key_down(unsigned int key);
		virtual void on_mouse_move(const npoint& pt, bool ctrl_down) = 0;
		virtual void zoom_extents(const AABBox& box, const ncamera* cam)
		{
			(void)box;
			(void)cam;
		}

		void set_factor(float move, float rotate, float wheel)
		{
			m_move_factor = move;
			m_rotate_factor = rotate;
			m_wheel_factor = wheel;
		}

		void set_move_factor(float f)	{	m_move_factor = f;}
		void set_rotate_factor(float f) {	m_rotate_factor = f;}
		void set_wheel_factor(float f) {	m_wheel_factor = f;}

		float get_move_factor() const {	return m_move_factor;}
		float get_rotate_factor() const {	return m_rotate_factor;}
		float get_wheel_factor() const {	return m_wheel_factor;}

	protected:
		npoint		m_left_drag_pt;
		bool		m_left_down;
		npoint		m_middle_drag_pt;
		bool		m_middle_down;
		npoint		m_right_drag_pt;
		bool		m_right_down;

		float		m_move_factor,
			m_rotate_factor,
			m_wheel_factor;
	};

	/**
	 * 使用Yaw，Pitch，Roll计算LookAt参数的助手类
	*/
	struct nAPI camera_YPR
	{	
		camera_YPR(void);
		~camera_YPR(void);

		void update_camera_view(ncamera* cam);

		void move_dist(float delta)				{	m_dist += delta;}
		void move_look_at(const vector3& delta)	{	m_look_at+=delta;}
		void move_yaw(float y)	{	m_yaw += y;}
		void move_pitch(float p)	{	m_pitch += p;}	
		void move_roll(float delta_roll)	{	m_roll += delta_roll;}	

		void set_look_at(const vector3& look_at)	{	m_last_look_at=m_look_at;	m_look_at = look_at;}
		void zoom_extents(const AABBox& box, const ncamera* cam);		

		vector3 get_direction() const;

		vector3	m_eye_pos;
		vector3	m_look_at,
			m_last_look_at;
		float	m_yaw,
				m_pitch,
				m_roll;
		float	m_dist;

		vector3	m_forward,
			m_up,
			m_side;
	};
}//namespace nexus
#endif //_NEXUS_CAMERA_CONTROLLER_H_