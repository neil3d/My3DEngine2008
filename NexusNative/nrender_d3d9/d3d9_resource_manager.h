/**
*	Nexus Engine - render resource manager
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_RESOURCE_MANAGER_H_
#define _NEXUS_D3D9_RESOURCE_MANAGER_H_
#include <boost/thread/recursive_mutex.hpp>

namespace nexus
{
	/**
	 * D3D9 Render Hardware Resource Manager
	*/
	class d3d9_resource_manager
		: public nrender_resource_manager
	{
	public:
		d3d9_resource_manager(void);
		~d3d9_resource_manager(void);

		virtual void destory();

		virtual nrender_static_mesh* alloc_static_mesh();
		virtual nrender_static_mesh_indexed* alloc_static_mesh_indexed();
		virtual nrender_anim_mesh* alloc_anim_mesh_indexed();
		virtual nrender_cpu_skin_mesh* alloc_cup_skin_mesh();
		virtual nrender_texture2D* alloc_texture_2d();
		virtual nrender_dynamic_mesh_indexed* alloc_dynamic_mesh_indexed();
		virtual nrender_dynamic_mesh* alloc_dynamic_mesh();
		virtual nrender_simple_mesh* alloc_simple_mesh();
		virtual nrender_font* alloc_render_font();
		virtual nui_canvas* alloc_ui_canvas();

		virtual nrender_heightmap* alloc_heightmap();
		virtual nrender_alphamap*	alloc_alphamap();
		virtual nrender_target* alloc_render_target();
		virtual nrender_cube_map*	alloc_cube_map();
		virtual nhit_proxy_hash* alloc_hit_proxy_hash();

		virtual void free_resource(nrender_resource* res);
	private:
		typedef std::set<nrender_resource*>	resource_set;
		
		boost::recursive_mutex				m_mutex;	// 因为资源本身有嵌套引用关系，所以这里选用了recursive mutex
		resource_set								m_resources;

		nDECLARE_CLASS(d3d9_resource_manager)
	};
}//namespace nexus
#endif //_NEXUS_D3D9_RESOURCE_MANAGER_H_