#include "StdAfx.h"
#include "camera_controller.h"

namespace nexus
{
	camera_YPR::camera_YPR(void):m_eye_pos(0,500,-500),
		m_look_at(0,0,0),
		m_forward(0,0,-1),
		m_up(0,1,0),
		m_side(1,0,0)
	{
		m_yaw = m_pitch = m_roll = 0;
		m_dist = 200;
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

		vector3 test = cam->get_eye_pos();

	}
}//namespace nexus