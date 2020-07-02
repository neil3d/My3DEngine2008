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
		m_lookat = at;

		mat_set_lookat_LH(m_mat_view, eye, at, up);
		m_mat_view_inv = mat_inverse(m_mat_view);
	}

	void ncamera::set_view(const matrix44& view_mat)
	{
		m_mat_view = view_mat;
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

	void ncamera::set_clip_plane(const plane& pln)
	{
		/*
			from NVIDIA SDK Code Sample : Clip Planes
		*/

		matrix44	mat_clip_proj;
		matrix44	mat_world2proj;

		mat_world2proj = m_mat_view*m_mat_project;

		// clip_plane is plane definition (world space)
		plane clip_plane = pln;
		clip_plane.normalize();

		mat_world2proj = mat_inverse(mat_world2proj);
		mat_world2proj = mat_transpose(mat_world2proj);

		vector4 vclip_plane(clip_plane.normal.x,
			clip_plane.normal.y,
			clip_plane.normal.z,
			clip_plane.dist);

		// transform clip plane into projection space
		vector4 proj_clip_plane = mat_transform(vclip_plane, mat_world2proj);
		mat_set_identity(mat_clip_proj);

		if( proj_clip_plane.w==0 )
			return; // plane is perpendicular to the near plane

		proj_clip_plane = vec_normalize(proj_clip_plane);

		if( proj_clip_plane.w > 0 )
		{
			proj_clip_plane = -proj_clip_plane;
			proj_clip_plane.w += 1;
		}

		// put projection space clip plane in Z column
		mat_clip_proj._13 = proj_clip_plane.x;
		mat_clip_proj._23 = proj_clip_plane.y;
		mat_clip_proj._33 = proj_clip_plane.z;
		mat_clip_proj._43 = proj_clip_plane.w;

		// multiply into projection matrix
		m_mat_project = m_mat_project*mat_clip_proj;
	}

	void ncamera::set_ortho(float w, float h, float zNear, float zFar)
	{
		m_znear = zNear;
		m_zfar = zFar;
		m_fov = -1;

		mat_set_ortho_LH(m_mat_project, w, h, zNear, zFar);
	}

	npoint ncamera::world2screen(const vector3& wpt) const
	{
		vector4 trans(wpt.x,wpt.y,wpt.z,1.0f);
		trans = mat_transform(trans, m_mat_view);
		trans = mat_transform(trans, m_mat_project);		
		trans /= trans.w;

		npoint ret;
		ret.x = long((trans.x+1.0f)*0.5f*float(m_viewport_w));
		ret.y = long(-(trans.y-1.0f)*0.5f*float(m_viewport_h));

		return ret;
	}
}//namespace nexus