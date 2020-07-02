/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDERER_BASE_H_
#define _NRENDERER_BASE_H_
#include <boost/function.hpp>
#include "../framework/nsubsystem.h"
#include "../framework/view_info.h"
#include "render_define.h"
#include "nrender_element.h"
#include "../scene/nscene_manager.h"

namespace nexus
{
	typedef boost::function<void(int)> handler_device_lost;
	typedef boost::function<bool(int)> handler_device_reset;

	class nimage;
	class nhit_proxy_hash;
	class ncamera;
	class nrender_target;
	struct nspt_instance;

	struct render_setting 
	{
		color4f skylight_low_color;
		color4f skylight_high_color;

		float		ssao_occlustion_radius;
		float		ssao_occlustion_power;
		float		ssao_blocker_power;

		render_setting():
		skylight_low_color(color4f(0.25f,0.25f,0.25f,1.0f))
		,skylight_high_color(color4f(0.65f,0.65f,0.65f,1.0f))
		,ssao_occlustion_radius(20)
		,ssao_occlustion_power(3.5)
		,ssao_blocker_power(1.5)
		{
		}

		void serialize(narchive& ar)
		{
			nSERIALIZE(ar,skylight_low_color);
			nSERIALIZE(ar,skylight_high_color);
			nSERIALIZE(ar,ssao_occlustion_radius);
			nSERIALIZE(ar,ssao_occlustion_power);
			nSERIALIZE(ar,ssao_blocker_power);
		}
	};

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
		virtual void close() = 0;
		virtual nstring get_resource_manager_class_name() = 0;
		virtual void set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name) = 0;
		virtual int register_device_handler(handler_device_lost hlost, handler_device_reset hreset) = 0;
		virtual void unregister_device_handler(int id) = 0;
		virtual bool is_device_lost() const = 0;
		virtual void render_scene(const nviewport& view,nscene_manager::ptr scene_ptr,const struct render_setting& setting) = 0;

		virtual void resolve_target(nrender_target* target) = 0;
		virtual void screen_shot(nimage* blank_img, size_t width, size_t height) = 0;
		virtual void back_surface_shot(nimage* blank_img) = 0;
		virtual void viewport_shot(nimage* blank_img) = 0;
		virtual void present(void* hWindow) = 0;

		nDECLARE_VIRTUAL_CLASS(nrenderer_base);
	};

	class render_package_base
	{
	public:
		render_package_base(void)	{}
		virtual ~render_package_base(void)	{}

		virtual void add_mesh(class nmesh_component* mesh) = 0;
		virtual void add_spt_tree(class nspeed_tree_component* tree) = 0;
		virtual void add_fog(class nfog_volume_component* fog) = 0;
		virtual void add_water(class nwater_component* water_comp) = 0;
		virtual void render_env_effect(const class nview_info* view,class drawing_policy* dp) = 0;
		virtual void draw_all(const nview_info* view,class drawing_policy* dp) = 0;
		virtual void draw_for_hit_hash(const nview_info* view,class drawing_policy* dp) = 0;
		virtual void begin(bool update_data = false) = 0;
		virtual void end() = 0;
		virtual void clear() = 0;
	};
}//namespace nexus
#endif //_NRENDERER_BASE_H_