/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDERER_BASE_H_
#define _NRENDERER_BASE_H_
#include "../framework/nsubsystem.h"
#include "../framework/view_info.h"
#include "nrender_proxy.h"
#include "nrender_light.h"
#include "render_define.h"
#include "nrender_element.h"

namespace nexus
{
	class nimage;
	class nhit_proxy_hash;
	class ncamera;
	class nrender_target;
	struct nspt_instance;
	/**
	 *	渲染模块基类
	*/
	class nAPI nrenderer_base :
		public nobject
	{
	public:
		typedef shared_ptr<nrenderer_base> ptr;

		nrenderer_base(void)	{}
		virtual ~nrenderer_base(void)	{}

		virtual void init(render_config& cfg) = 0;
		virtual void close() = 0;
		virtual nstring get_resource_manager_class_name() = 0;
		virtual void set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name) = 0;

		virtual bool frame_begin(const nviewport& view) = 0;
		virtual void frame_begin_level(const ncamera* cam) = 0;
		virtual void frame_object(const nrender_proxy* obj) = 0;
		virtual void frame_tree(const nspt_instance* spt) = 0;	// speed tree特殊渲染流程
		virtual void frame_light(const nrender_light_proxy* lgt) = 0;		
		virtual void frame_end_level(nhit_proxy_hash* hit_map) = 0;
		virtual void resolve_target(nrender_target* target) = 0;
		virtual void screen_shot(nimage* blank_img) = 0;
		virtual void viewport_shot(nimage* blank_img) = 0;
		virtual void frame_end() = 0;
		virtual void present(void* hWindow) = 0;

		nDECLARE_VIRTUAL_CLASS(nrenderer_base);
	};
}//namespace nexus
#endif //_NRENDERER_BASE_H_