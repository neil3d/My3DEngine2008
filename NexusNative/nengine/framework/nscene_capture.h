/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/
#ifndef _NEXUS_SCENE_CAPTURE_H_
#define _NEXUS_SCENE_CAPTURE_H_
#include "view_info.h"

namespace nexus
{
	class nlevel;
	class nrender_target;

	/**
	 *	Render to texture
	 *	@remarks 内部应控制：视锥剪裁；场景LOD bias；渲染频率等
	*/
	class nscene_capture
	{
	public:
		typedef boost::shared_ptr<nscene_capture> ptr;

		nscene_capture(void);
		virtual ~nscene_capture(void);

		/** determin if a capture is needed on the given view
		 *	@remarks 内部可能计算FPS控制，视锥剪裁等；
		*/
		virtual bool update_required(const nviewport& view) = 0;

		/** 
		 *	render the scene for the capture
		*/
		virtual void capture_scene(nlevel* lv) = 0;

		virtual nrender_target* get_texture_target() = 0;
	};
}//namespace nexus
#endif //_NEXUS_SCENE_CAPTURE_H_