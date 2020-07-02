/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	April, 2010
*/
#ifndef _NEXUS_TERRAIN_CHUNK_H_
#define _NEXUS_TERRAIN_CHUNK_H_
#include "../actor/nmesh_component.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "nterrain_mtl.h"

namespace nexus
{
	class nterrain_mtl_setup;
	class nterrain_actor;
	class nterrain_chunk_vertex_data;
	class nterrain_deco_layer;
	class nterrain_deco_chunk_base;

	/** 地形块Mesh使用Static Mesh的适配器
	 *	@remarks 根据LOD选择不同的Section
	*/
	class nterrain_chunk_mesh_adapter
		: public nrender_mesh
	{
	public:
		nterrain_chunk_mesh_adapter(void)	{}
		virtual ~nterrain_chunk_mesh_adapter(void)	{}

		//! 创建内部的static mesh对象，以及内容
		void create(const nmesh_vertex_data* vert_data, const index_buffer16* index_data);

		//! 使用interlocked tile方法创建lod时，添加多个index buffer
		void add_interlocked_tile_lod(const index_buffer16* index_data);

		//! 设置chunk参数
		void set_chunk(int chunk_size, int terrain_w, int terrain_h);

		//-- 父类接口
		virtual nshader_modifier* get_shader_mod()		{	return m_render_mesh->get_shader_mod(); }
		virtual nvertex_factory* get_vertex_factory()	{	return m_render_mesh->get_vertex_factory();}
		virtual size_t get_num_section()				{	return 1;}
		virtual nrender_mesh_section* get_section(size_t section_index);
		virtual bool pre_render()	{	return m_render_mesh->pre_render(); }
		virtual void post_render()	{	return m_render_mesh->post_render(); }

		virtual void draw_setup_effect(nshading_effect* effect_ptr);	

		virtual void update_vertex_buffer(const nmesh_vertex_data* vert_data)
		{
			m_render_mesh->update_vertex_buffer(vert_data);
		}

		void destroy()
		{
			m_render_mesh.reset();
		}

	private:
		render_res_ptr<nrender_static_mesh_indexed>	m_render_mesh;
		vector2	m_chunk_uv_scale;
	};

	/**
	 *	渲染一个地形块所使用的component
	*/
	class nterrain_chunk
		: public nmesh_component
	{
	public:
		typedef boost::shared_ptr<nterrain_chunk> ptr;

		nterrain_chunk(const nstring& name_str);
		virtual ~nterrain_chunk(void);

		//! 创建内部渲染用的mesh对象
		void create(int x, int y, int chunk_size);

		//! 在高度图产生变化时，重新计算vertex buffer，以及bounding box
		void post_heightmap_change(const nrect& region);

		//!	在viz map改变时，重新计算index buffer
		void post_vizmap_change(const nrect& region);

		//! 通知创建内部材质对象
		void _post_material_create(nterrain_mtl_setup* mtl);

		//! 当图层设置发生变化时更新内部材质对象
		void _post_layer_change();

		//! 当图层的alpha发生变化时更新内部材质对象
		void _post_layer_alpha_change(const nstring& layer_name, const nrect& region);

		//! 添加一个装饰层
		void	add_deco_layer(nterrain_deco_layer* layer);

		//删除一个装饰层
		void	del_deco_layer(nterrain_deco_layer* layer);

		//装饰层发生变化
		void	_post_deco_layer_change();

		//! 计算chunk覆盖地形的那个矩形区域
		nrect get_map_rect() const
		{
			return nrect(m_hmap_x, m_hmap_y, m_hmap_x+m_chunk_size, m_hmap_y+m_chunk_size);
		}

		//-- 父类接口
		virtual nmtl_base* get_material(int lod, int mtl_id);
		
		void get_mesh_element(int lod,vector<mesh_element>& mesh_elements);

		virtual const matrix44& get_world_matrix() const
		{
			return m_owner->get_world_matrix();
		}
		virtual void _update_transform(const object_space& parent_space);
		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);
	private:
		void update_mesh_element();
		void update_vertex_data(nterrain_chunk_vertex_data* vert_buffer, nsize& out_height_range);
		void update_index_buffer(index_buffer16* indices);
		unsigned short get_index(int j, int i) const
		{
			return j*m_chunk_size+i;
		}

		void update_bounds(nsize height_range);
		void create_render_mesh();
	
	private:
		int	m_hmap_x,
			m_hmap_y;
		int	m_chunk_size;

		nterrain_chunk_mesh_adapter	m_render_mesh;
		nterrain_mtl_basic::ptr		m_mtl_low;	// 使用nterrain_mtl_basic作为低精度材质
		ntexture_splatting::ptr		m_mtl_high;	// 使用ntexture_splatting作为高精度材质

		vector<boost::shared_ptr<nterrain_deco_chunk_base> >	m_deco_chunk_list;

		nDECLARE_NAMED_CLASS(nterrain_chunk)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_CHUNK_H_