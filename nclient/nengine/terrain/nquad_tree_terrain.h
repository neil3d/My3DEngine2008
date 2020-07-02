/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_QUAD_TERRAIN_TERRAIN_H_
#define _NEXUS_QUAD_TERRAIN_TERRAIN_H_
#include <boost/pool/object_pool.hpp>
#include "../actor/nprimitive_component.h"
#include "../mesh/mesh_vertex_data.h"
#include "../renderer/nrender_mesh.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../renderer/nrender_resource.h"
#include "nterrain_mtl.h"
#include "nheight_map.h"

namespace nexus
{
	/**
	*	地形顶点数据
	*	@remakrs 实现顶点buffer的填充; 指定vertex factory.
	*/
	class nterrain_chunk_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nterrain_chunk_vertex_data(void)	{}
		virtual ~nterrain_chunk_vertex_data(void)	{}

		virtual size_t get_num_stream() const	{	return 1; }
		virtual vertex_stream* get_stream(size_t stream_index)
		{
			(void)stream_index;
			return &m_pos_stream;
		}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		void init(size_t num_rows, size_t num_cols, bool enable_skirt);
	};

	/**
	*	static mesh adapter for terrain chunk
	*	@remarks 内部数据处理使用标准的static mesh，但是增加了特殊的effect参数
	*/
	class nterrain_chunk_render_mesh
		: public nrender_mesh
	{
	public:
		nterrain_chunk_render_mesh(void);
		virtual ~nterrain_chunk_render_mesh(void);

		void init(nrender_static_mesh_indexed* chunk_mesh, nrender_heightmap* tex)
		{			
			m_chunk_mesh = chunk_mesh;
			m_heightmap_tex = tex;
		}
		void set_quad_factor(float uBias, float vBias, float scale)
		{
			m_quad_factor.x = uBias;
			m_quad_factor.y = vBias;
			m_quad_factor.z = scale;
		}

		void set_texel_size(int level, size_t hmap_w, size_t hmap_h)
		{
			m_texel_size.x = (1<<level) / float(hmap_w-1);
			m_texel_size.y = (1<<level) / float(hmap_h-1);
			m_texel_size.z = 1;
			m_texel_size.w = 0;
		}

		virtual nvertex_factory* get_vertex_factory()	{	return m_chunk_mesh->get_vertex_factory(); }
		virtual size_t get_num_section()	{	return m_chunk_mesh->get_num_section(); }
		virtual nrender_mesh_section* get_section(size_t section_index) {	return m_chunk_mesh->get_section(section_index);}

		virtual void draw_setup_effect(nshading_effect* effect_ptr);
	private:
		nrender_static_mesh_indexed*	m_chunk_mesh;
		nrender_heightmap*				m_heightmap_tex;		
		
		vector3		m_quad_factor;
		vector4		m_texel_size;
	};

	/**
	 *	使用quad tree算法渲染地形
	 *	@remarks 管理material，render mesh等对象
	 *	@todo: 尝试使用interlocked tile来解决接缝问题
	*/
	class nquad_tree_terrain
		: public nprimitive_component
	{
	public:
		nquad_tree_terrain(const nstring& name_str);
		virtual ~nquad_tree_terrain(void);

		void create(nheight_map16* hmap, size_t chunk_size);
		void post_heightmap_change(const nrect& rc, nheight_map16* hmap);

		void create_material_basic(const resource_location& texture_loc);
		ntexture_splatting::ptr create_texture_splatting(size_t alpha_w, size_t alpha_h);

		void set_scale(const vector3& scale);
		void set_position(const vector3& pos);

		virtual void render(const view_info& view);

		transparent_flag get_transparent_flag() const	{	return m_trans_flag; }
		virtual nmaterial_base* get_material() const;
	protected:
		void render_quad(float minU, float minV, float maxU, float maxV, int level, float scale, const view_info& view);
		bool lod_split(float minU, float minV, float maxU, float maxV, const view_info& view);
		void draw_mesh(float uBias, float vBias, float scale, int level);
		//--
		void create_shared_mesh();		
		void fill_indices(index_buffer16& index_data);
		void fill_indices_no_skirt(index_buffer16& index_data);
		
		void update_bounds();
		void update_world_matrix();

	protected:
		nheight_map16*		m_heightmap;
		size_t				m_chunk_size,
							m_max_lv;
		vector3				m_scale,
							m_pos;

		nterrain_mtl::ptr	m_material;
		transparent_flag	m_trans_flag;
		
		render_res_ptr<nrender_static_mesh_indexed>
							m_chunk_mesh;
		render_res_ptr<nrender_static_mesh_indexed>
							m_wireframe_mesh;

		render_res_ptr<nrender_heightmap>
							m_heightmap_tex;

	private:
		struct trn_render_proxy
			: public nrender_proxy
		{			
			typedef shared_ptr<trn_render_proxy> ptr;

			nquad_tree_terrain*				m_trn;
			mutable nterrain_chunk_render_mesh		m_mesh;

			virtual const matrix44& get_world_matrix() const	{	return m_trn->get_world_matrix(); }
			virtual int get_render_lod() const					{	return 0; }
			virtual enum EDepthGroup get_depth_group() const	{	return EDG_World; }	
			virtual nrender_mesh* get_render_mesh(int lod) const
			{
				(void)lod;
				return &m_mesh;
			}

			virtual nmaterial_base* get_material(int lod, int mtl_id) const
			{
				(void)lod;
				(void)mtl_id;
				return m_trn->get_material();
			}
			virtual transparent_flag get_transparent_flag() const	{	return m_trn->get_transparent_flag(); }
			virtual const box_sphere_bounds& get_bounds() const		{	return m_trn->get_bounds(); }

		};

		boost::object_pool<trn_render_proxy>	m_render_pool;
		std::vector<trn_render_proxy::ptr>		m_render_nodes;

		void free_render_node(trn_render_proxy* node)
		{
			m_render_pool.destroy(node);
		}
		nDECLARE_NAMED_CLASS(nquad_tree_terrain)
	};
}//namespace nexus

#endif //_NEXUS_QUAD_TERRAIN_TERRAIN_H_