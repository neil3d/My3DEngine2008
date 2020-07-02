#include "stdafx.h"
#include "ncamera.h"

namespace nexus
{
	ncamera::ncamera(void)
	{
		mat_set_identity(m_mat_view);
		mat_set_identity(m_mat_view_inv);
		mat_set_identity(m_mat_project);
	}

	ncamera::~ncamera(void)
	{
	}

	void ncamera::set_lookat(const vector3& eye, const vector3& at, const vector3& up)
	{
		mat_set_lookat_LH(m_mat_view, eye, at, up);
		m_mat_view_inv = mat_inverse(m_mat_view);
	}

	void ncamera::set_perspective(float FOV, int viewport_w, int viewport_h, float zNear, float zFar)
	{
		float aspect = (float)viewport_w/viewport_h;
		mat_set_perspective_LH(m_mat_project, FOV, aspect, zNear, zFar);

		m_fov = FOV;
		m_viewport_w = viewport_w;
		m_viewport_h = viewport_h;
		m_znear = zNear;
		m_zfar = zFar;
	}


	// The rows of a square matrix can be interpreted as the basis vectors of a coordinate space
	vector3 ncamera::get_view_x() const
	{
		return m_mat_view.get_axis_x();
	}

	vector3 ncamera::get_view_y() const
	{
		return m_mat_view.get_axis_y();
	}

	vector3 ncamera::get_view_z() const
	{
		return m_mat_view.get_axis_z();
	}

	vector3 ncamera::get_eye_pos() const
	{
		return m_mat_view_inv.get_origin();
	}

	vector3 ncamera::screen2world(int screen_x, int screen_y) const
	{
		float hw, hh;
		float aspect = (float)m_viewport_w/m_viewport_h;

		hw = tanf(m_fov*0.5f)*m_znear*aspect;
		hh = hw/aspect;

		vector3 r(screen_x/(m_viewport_w*0.5f)-1.0f,
			1.0f-screen_y/(m_viewport_h*0.5f),m_znear);

		r.x *= hw;
		r.y *= hh;
		
		return r*m_mat_view_inv;
	}

	void ncamera::get_mouse_ray(int cursorX, int cursorY, ray& out) const
	{
		out.origin = screen2world(cursorX,cursorY);
		out.dir = vec_normalize( out.origin-get_eye_pos() );	//从From点出发,指向"鼠标所指向的点"的方向

		out.origin += out.dir*m_znear;//进行near剪裁
	}
}//namespace nexus