/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDERER_BASE_H_
#define _NRENDERER_BASE_H_
#include "../framework/nsubsystem.h"
#include "nrender_proxy.h"
#include "nrender_light.h"
#include "render_define.h"
#include "nrender_element.h"

namespace nexus
{
	class ncamera;
	/**
	 *	äÖÈ¾Ä£¿é»ùÀà
	*/
	class nAPI nrenderer_base :
		public nobject
	{
	public:
		typedef shared_ptr<nrenderer_base> ptr;

		nrenderer_base(void)	{}
		virtual ~nrenderer_base(void)	{}

		virtual void init(render_config& cfg) = 0;
		virtual nstring get_resource_manager_class_name() = 0;
		virtual void set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name) = 0;

		virtual void set_render_mode(enum ERenderMode rm) = 0;
		virtual enum ERenderMode get_render_mode() const = 0;

		virtual bool frame_begin(const ncamera* cam) = 0;
		virtual void frame_object(const nrender_proxy* obj) = 0;
		virtual void frame_light(const nrender_light_proxy* lgt) = 0;
		virtual void frame_end() = 0;
		virtual void present(void* hWindow) = 0;

		virtual void attach_element(nrender_element::ptr element_ptr) = 0;
		virtual void detach_element(nrender_element::ptr element_ptr) = 0;

		nDECLARE_VIRTUAL_CLASS(nrenderer_base);
	};
}//namespace nexus
#endif //_NRENDERER_BASE_H_