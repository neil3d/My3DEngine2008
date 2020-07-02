/**
*	nexus core - camera
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_CAMERA_H_
#define _NEXUS_CAMERA_H_
#include "matrix.h"
#include "geom_def.h"
#include "rect.h"

namespace nexus
{
	/**
	 *	基本的摄像机封装
	*/
	class nCoreAPI ncamera
	{
	public:
		ncamera(void);
		~ncamera(void);

		/** 设置view矩阵 */
		void set_lookat(const vector3& eye, const vector3& at, const vector3& up);
		/** 设置透视投影矩阵 */
		void set_perspective(float FOV, int viewport_w, int viewport_h, float zNear, float zFar);
		/** w，h等参数均为camera space中的距离值*/
		void set_ortho(float w, float h, float zNear, float zFar);

		const matrix44& get_view() const	{	return m_mat_view;}
		const matrix44& get_project() const	{	return m_mat_project;}

		void set_view(const matrix44& view_mat);
		void set_clip_plane(const plane& pln);
		
		vector3 get_eye_pos() const;
		vector3 get_view_x() const;	// side
		vector3 get_view_y() const;	// up
		vector3 get_view_z() const;	// forward
		vector3 get_lookat() const	{	return m_lookat; }
		float get_zfar() const	{	return m_zfar;}
		float get_znear() const	{	return m_znear;}
		float get_fov() const	{	return m_fov;}

		vector3 screen2world(int screen_x, int screen_y) const;
		npoint world2screen(const vector3& p) const;
		void get_mouse_ray(int cursorX, int cursorY, ray& out) const;

		int get_viewport_width() const	{	return m_viewport_w;}
		int get_viewport_height() const	{	return m_viewport_h;}
		float get_viewport_aspect() const
		{
			return (float)m_viewport_w/m_viewport_h;
		}

	protected:
		matrix44	m_mat_view;
		matrix44	m_mat_view_inv;
		matrix44	m_mat_project;		

		float	m_fov;
		float	m_znear;
		float	m_zfar;

		vector3	m_lookat;
		int		m_viewport_w,
				m_viewport_h;
	};
}//namespace nexus
#endif //_NEXUS_CAMERA_H_