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
#include "drawing_policy.h"
#include "d3d9_PDI.h"

namespace nexus
{
	typedef std::vector<const nrender_light_proxy*>	light_vector;
	typedef std::vector<const nrender_proxy*> render_proxy_vector;
	typedef std::vector<nrender_mesh*> render_mesh_vector;

	/**
	 *	 Primary Interface of D3D9 Renderer
	*/
	class nrenderer_d3d9 : public nrenderer_base
	{
	public:
		nrenderer_d3d9(void);
		virtual ~nrenderer_d3d9(void);

		virtual void init(render_config& cfg);
		virtual nstring get_resource_manager_class_name()	{	return _T("d3d9_resource_manager"); }
		virtual void set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name);

		virtual void set_render_mode(enum ERenderMode rm)	{	m_render_mode = rm;}
		virtual enum ERenderMode get_render_mode() const	{	return m_render_mode; }
		virtual bool frame_begin(const ncamera* cam);
		virtual void frame_object(const nrender_proxy* obj);
		virtual void frame_light(const nrender_light_proxy* lgt);
		virtual void frame_end();
		virtual void present(void* hWindow);

		virtual void attach_element(nrender_element::ptr element_ptr)
		{
			m_elements.push_back( element_ptr );
		}

		virtual void detach_element(nrender_element::ptr element_ptr)
		{
			m_elements.remove(element_ptr);
		}

	protected:
		void frame_view_info(const ncamera* cam);
		void close();
		void frame_render();
		void draw_pass(drawing_policy* dp);
		void render_lights();

	private:
		d3d_view_info			m_view_info;

		enum ERenderMode	m_render_mode;		
		render_group_world	m_world_group;		
		render_group*		m_render_group[EDG_Max];

		light_vector		m_lights;
		render_mesh_vector	m_post_handle_array;

		d3d9_PDI			m_PDI;
		std::list<nrender_element::ptr>	m_elements;

		nDECLARE_CLASS(nrenderer_d3d9)
	};

	extern nfile_system::ptr g_file_sys;
	extern nstring	g_engine_package_name;
}//namespace nexus
#endif //_NEXUS_RENDERER_D3D9_H_