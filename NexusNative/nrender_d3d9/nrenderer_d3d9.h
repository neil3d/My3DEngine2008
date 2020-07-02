/**
*	Nexus Engine - 渲染模块主接口类
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDERER_D3D9_H_
#define _NEXUS_RENDERER_D3D9_H_

#include "../nengine/framework/view_info.h"
#include "nrender_light.h"
#include "d3d9_PDI.h"
#include "render_package.h"

namespace nexus
{
	typedef std::vector<nrender_light_proxy*>	light_vector;
	typedef std::vector<nrender_mesh*> render_mesh_vector;
	typedef const nscene_manager::prim_octree_type::octreenode_ptr	 node_ptr;
	struct render_setting;

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
		virtual int register_device_handler(handler_device_lost hlost, handler_device_reset hreset);
		virtual void unregister_device_handler(int id);
		virtual bool is_device_lost() const
		{
			return m_device_lost!=0;
		}

		virtual void render_scene(const nviewport& view,nscene_manager::ptr scene_ptr,const render_setting& setting);

		virtual void frame_light(const nlight_component* lgt);	
		virtual void resolve_target(nrender_target* target);
		virtual void screen_shot(nimage* blank_img, size_t width, size_t height);
		virtual void back_surface_shot(nimage* blank_img);
		virtual void viewport_shot(nimage* blank_img);
		virtual void present(void* hWindow);	

		float get_run_time()
		{
			return (float)m_timer.elapsed();
		}

		virtual IDirect3DBaseTexture9* get_texture(EEngineTexture ETex);

		const nview_info* get_view_info() const	{	return &m_view_info; }
	protected:
		void frame_view_info(const nviewport& view);		
		void update_hit_proxy_hash(nhit_proxy_hash* hit_map);
		void draw_background();
		void draw_world();
		void draw_foreground();
		void render_lights();
		void render_postprocess();
		void make_occlusion_query( const nview_info&view_info,nprimitive_component* prim );
		void make_occlusion_query( const nview_info&view_info,node_ptr node,box_sphere_bounds bound);

	private:
		nview_info			m_view_info;
		nscene_manager::ptr m_scene_ptr;
		nrender_element::ptr	 m_view_widgets;

		enum ERenderMode	m_render_mode;				
	
		light_vector		m_lights;

		vector<const class post_process*> m_postprocess_array;

		class ssao*		m_ssao;
		d3d9_PDI			m_PDI;		
		
		boost::timer	 m_timer;
		volatile LONG	m_device_lost;	

		render_package<> m_render_package;
		render_setting m_render_setting;

		vector<nprimitive_component*> in_frustum_prims;
		typedef std::map<nprimitive_component*,class occlusion_query*> occlusion_map;
		typedef std::map<node_ptr,class occlusion_query*> occlusion_node_map;
		occlusion_map m_occlusion_map;
		occlusion_node_map m_occlusion_node_map;
		bool bocclusion_query;

		IDirect3DBaseTexture9*				m_engine_textures[ETexture_Max];

		friend class npoint_light_proxy;
		friend class ndirectional_light_proxy;
		friend class nspot_light_proxy;	
		nDECLARE_CLASS(nrenderer_d3d9)
	};

	extern nfile_system::ptr g_file_sys;
	extern nstring	g_engine_package_name;
	//绘制一个几何体，将几何体内的像素用蒙板标示出
	extern void draw_stencil_volume(const vector<vector3>& positons,
									const UINT* indexes,
									UINT prim_count, 
									const matrix44&world,
									const matrix44&view,
									const matrix44&proj
									);
}//namespace nexus
#endif //_NEXUS_RENDERER_D3D9_H_