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
		camera_controller(void)	{	}
		virtual ~camera_controller(void)	{	}

		virtual void update_camera(ncamera* cam) = 0;
	};

	/**
	 * 使用Yaw，Pitch，Roll计算LookAt参数的助手累
	*/
	struct camera_YPR
	{	
		camera_YPR(void);
		~camera_YPR(void);

		void update_camera_view(ncamera* cam);

		void move_dist(float delta)				{	m_dist += delta;}
		void move_look_at(const vector3& delta)	{	m_look_at+=delta;}
		void move_yaw(float y)	{	m_yaw += y;}
		void move_pitch(float p)	{	m_pitch += p;}	

		void set_look_at(const vector3& look_at)	{	m_look_at = look_at;}

		vector3	m_eye_pos;
		vector3	m_look_at;
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