#include "StdAfx.h"
#include "camera_controller.h"

namespace nexus
{
	//-- class camera_controller --------------------------------------------------------------------------------------
	camera_controller::camera_controller(void) : m_left_down(0), m_right_down(0),m_middle_down(0),
		m_move_factor(4), m_rotate_factor(0.005f), m_wheel_factor(0.5f)
	{}

	void camera_controller::on_mouse_left_down(const npoint& pt)
	{
		m_left_down = true;
		m_left_drag_pt = pt;
	}

	void camera_controller::on_mouse_left_up()
	{
		m_left_down = false;
	}

	void camera_controller::on_mouse_right_down(const npoint& pt)
	{
		m_right_down = true;
		m_right_drag_pt = pt;
	}

	void camera_controller::on_mouse_right_up()
	{
		m_right_down = false;
	}

	void camera_controller::on_mouse_middle_down( const npoint& pt )
	{
		m_middle_down = true;
		m_middle_drag_pt = pt;
	}

	void camera_controller::on_mouse_middle_up()
	{
		m_middle_down = false;
	}

	bool camera_controller::on_key_down( unsigned int key )
	{
		(void)(key);
		return false;
	}
	//-- class camera_YPR --------------------------------------------------------------------------------------
	camera_YPR::camera_YPR(void):m_eye_pos(0,500,-500),
		m_look_at(0,0,0),
		m_last_look_at(0,0,0),
		m_forward(0,0,-1),
		m_up(0,1,0),
		m_side(1,0,0)
	{
		m_yaw = m_pitch = m_roll = 0;
		m_dist = 2000;
	}

	camera_YPR::~camera_YPR(void)
	{
	}

	void camera_YPR::update_camera_view(ncamera* cam)
	{
		// 计算内部数据
		float cosY, cosP, cosR;
		float sinY, sinP, sinR;

		cosY = cosf(m_yaw);
		cosP = cosf(m_pitch);
		cosR = cosf(m_roll);

		sinY = sinf(m_yaw);
		sinP = sinf(m_pitch);
		sinR = sinf(m_roll);

		m_forward.x	= sinY*cosP;
		m_forward.y = sinP;
		m_forward.z = cosP* -cosY;

		m_eye_pos = m_look_at - m_forward*m_dist;

		m_up.x = -cosY*sinR - sinY*sinP*cosR;
		m_up.y = cosP*cosR;
		m_up.z = -sinY*sinR - sinP*cosR*-cosY;

		m_side = vec_cross(m_forward, m_up);

		// 更新摄像机矩阵
		cam->set_lookat(m_eye_pos, m_look_at, m_up);

		//vector3 test = cam->get_eye_pos();
	}

	void camera_YPR::zoom_extents(const AABBox& box, const ncamera* cam)
	{
		vector3 center = (box.m_max+box.m_min)*0.5f;
		vector3 extent = box.m_max-box.m_min;
		float e = std::max(extent.x, extent.y);
		e = std::max(e, extent.z);

		//m_yaw = m_pitch = m_roll = 0;

		m_look_at = center;
		m_dist = e/tanf(cam->get_fov()*0.5f);
	}

	vector3 camera_YPR::get_direction() const
	{
		return vec_normalize(m_look_at - m_eye_pos);
	}
}//namespace nexus