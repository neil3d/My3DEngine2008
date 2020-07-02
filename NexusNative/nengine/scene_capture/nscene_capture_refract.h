/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SCENE_CAPTURE_REFRACT_H_
#define _NEXUS_SCENE_CAPTURE_REFRACT_H_
#include "ncore.h"
#include "../framework/nscene_capture.h"
#include "../renderer//nrender_resource.h"
#include "../renderer/nrender_target.h"

namespace nexus
{
	/**
	 * render the scene to a 2d texture using a clip plane
	*/
	class nscene_capture_refract
		: public nscene_capture
	{
	public:
		typedef boost::shared_ptr<nscene_capture_refract> ptr;

		nscene_capture_refract(void);
		virtual ~nscene_capture_refract(void);

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

	protected:
		plane		m_plane;		
		nviewport	m_view;

		matrix44	m_uv_tranfo;
		matrix44	m_uv_project;

		render_res_ptr<nrender_target>	m_rt;
	};
}//namespace nexus
#endif //_NEXUS_SCENE_CAPTURE_REFRACT_H_