/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SCENE_CAPTURE_REFLECT_H_
#define _NEXUS_SCENE_CAPTURE_REFLECT_H_
#include "ncore.h"
#include "../framework/nscene_capture.h"
#include "../renderer//nrender_resource.h"
#include "../renderer/nrender_target.h"

namespace nexus
{
	/**
	 *	renders a scene as a reflection to a 2d texture target
	*/
	class nscene_capture_reflect
		: public nscene_capture
	{
	public:
		typedef boost::shared_ptr<nscene_capture_reflect> ptr;

		nscene_capture_reflect(void);
		~nscene_capture_reflect(void);

		void create(int render_target_w, int render_target_h);
		void set_plane(const plane& pln)
		{
			m_plane = pln;
			m_plane.normalize();			
		}

		virtual bool update_required(const nviewport& view);
		virtual void capture_scene(nlevel* lv);
		virtual nrender_target* get_texture_target();

		const matrix44& get_uv_project_matrix() const	{	return m_uv_project; }

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

	protected:
		plane		m_plane;		
		nviewport	m_view;
		
		matrix44	m_uv_tranfo;
		matrix44	m_uv_project;
		
		render_res_ptr<nrender_target>	m_rt;
	};
}//namespace nexus
#endif //_NEXUS_SCENE_CAPTURE_REFLECT_H_