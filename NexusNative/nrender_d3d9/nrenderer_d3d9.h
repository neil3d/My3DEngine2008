/**
*	Nexus Engine - 渲染模块主接口类
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDERER_D3D9_H_
#define _NEXUS_RENDERER_D3D9_H_

#include "d3d_view_info.h"
#include "render_group_world.h"
#include "render_group_plain.h"
#include "drawing_policy.h"
#include "d3d9_PDI.h"
#include "shadow_buffer.h"
#include "nspeed_tree_renderer.h"

namespace nexus
{
	typedef std::vector<const nrender_light_proxy*>	light_vector;
	typedef std::vector<const nrender_proxy*> render_proxy_vector;
	typedef std::vector<nrender_mesh*> render_mesh_vector;

	class nimage;
	/**
	 *	 Primary Interface of D3D9 Renderer
	*/
	class nrenderer_d3d9 : public nrenderer_base
	{
	public:
		nrenderer_d3d9(void);
		virtual ~nrenderer_d3d9(void);

		static nrenderer_d3d9* instance();

		virtual void init(render_config& cfg);
		virtual void close();
		virtual nstring get_resource_manager_class_name()	{	return _T("d3d9_resource_manager"); }
		virtual void set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name);

		virtual bool frame_begin(const nviewport& view);
		virtual void frame_begin_level(const ncamera* cam);
		virtual void frame_object(const nrender_proxy* obj);
		virtual void frame_tree(const nspt_instance* spt);
		virtual void frame_light(const nrender_light_proxy* lgt);		
		virtual void frame_end_level(nhit_proxy_hash* hit_map);
		virtual void resolve_target(nrender_target* target);
		virtual void screen_shot(nimage* blank_img);
		virtual void viewport_shot(nimage* blank_img);
		virtual void frame_end();
		virtual void present(void* hWindow);	

		float get_run_time()
		{
				return (float)m_timer.elapsed();
		}

		const d3d_view_info* get_view_info() const	{	return &m_view_info; }

	protected:
		void frame_view_info(const ncamera* cam);		
		void frame_render();		
		void update_hit_proxy_hash(nhit_proxy_hash* hit_map);
		void draw_background();
		void draw_world();
		void draw_foreground();
		void generate_shadow_buffer(const ndirectional_light_proxy* lgt);
		void render_lights();

	private:
		d3d_view_info			m_view_info;
		nrender_element::ptr	m_view_widgets;

		enum ERenderMode	m_render_mode;		
		render_group_world	m_world_group;		
		render_group_plain	m_bg_group;		// background group
		render_group_plain	m_fore_group;	// foreground group
		render_group*		m_render_group[EDG_Max];

		light_vector		m_lights;
		render_mesh_vector	m_post_handle_array;

		d3d9_PDI			m_PDI;		
		nspeed_tree_renderer	m_spt_renderer;

		boost::scoped_ptr<shadow_buffer>	m_shadow_buffer;
		boost::timer	m_timer;

		nDECLARE_CLASS(nrenderer_d3d9)
	};

	extern nfile_system::ptr g_file_sys;
	extern nstring	g_engine_package_name;
}//namespace nexus
#endif //_NEXUS_RENDERER_D3D9_H_