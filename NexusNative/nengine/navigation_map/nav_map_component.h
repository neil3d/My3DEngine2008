/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_
#define _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_
#include "../actor/nmesh_component.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../material/nmtl_static.h"

namespace nexus
{	
	class nav_map_actor;
	class nav_map_vertex_data;

	/**
	* 导航图采用类似于地形的分块渲染方式，这里描述了导航图中单个块
	*/
	class nAPI nav_map_component :
		public nmesh_component
	{
	public:
		typedef boost::shared_ptr<nav_map_component> ptr;

		nav_map_component(const nstring& name_str);
		virtual ~nav_map_component(void);

		// 初始化，生成VertBuffer & IndexBuffer数据
		void create(nav_map_actor* nav, int x, int y, int chunk_size, float cull_dist);
		// 高度图发生变化，更新顶点数据
		void post_heightmap_change(const nrect& region);

		// 重载基类接口
		virtual void _destroy();

		virtual nmtl_base* get_material(int lod, int mtl_id);		

		virtual const matrix44& get_world_matrix() const;
		virtual void _update_transform(const object_space& parent_space);

		virtual void serialize(narchive& ar);

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

	private:
		void update_vertex_data(nav_map_vertex_data* vert_buffer, nsize& out_height_range);
		void update_index_buffer(index_buffer16* indices);

		unsigned short get_index(int j, int i) const
		{
			return j*m_chunk_size+i;
		}

		void update_bounds(nsize height_range);
		void create_render_mesh();

	private:
		render_res_ptr<nrender_static_mesh_indexed>	m_mesh_ptr;
		int	m_hmap_x,
			m_hmap_y;
		int	m_chunk_size;

		// 渲染顶点颜色用的材质
		nmtl_static	m_mtl;

		nDECLARE_NAMED_CLASS(nav_map_component)
	};

} // end of namespace nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_COMPONENT_H_