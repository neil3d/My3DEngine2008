#ifndef _NEXUS_CHUNKED_TERRAIN_IMPL_H_
#define _NEXUS_CHUNKED_TERRAIN_IMPL_H_
#include "../mesh/mesh_vertex_data.h"
#include "../actor/nprimitive_component.h"
#include "../renderer/nrender_mesh.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../renderer/nrender_resource.h"
#include "nheight_map.h"
#include "nterrain_mtl.h"

namespace nexus
{
	class nterrain_chunk;

	/**
	*	地形顶点数据
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

		void init(size_t num_vert)
		{
			init_pos_stream(num_vert);
		}
	};


	class nchunked_terrain_impl
	{
		enum EConst
		{
			MAX_ALPHA_LAYER = 4,
		};
	public:
		nchunked_terrain_impl(void);
		~nchunked_terrain_impl(void);

		void create(size_t w, size_t h, short init_h, size_t chunk_size);
		void destroy();
		void create_material_basic(const resource_location& texture_loc);

		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
		void import_heightmap(const nstring& img_file_name);
		void post_height_changed(const nrect& rc);

		nmaterial_base* get_material() const;
		nrender_static_mesh_indexed* get_chunk_mesh() const	{	return m_chunk_mesh.get();}
		nrender_heightmap* get_heightmap_texture() const	{	return m_heightmap_tex.get();}
	protected:		
		typedef unsigned char	alpha_type;

		nheight_map16				m_height_map;
		nheight_map<alpha_type>		m_layer_map[MAX_ALPHA_LAYER];
		nterrain_mtl::ptr			m_material;

	private:
		nterrain_chunk_vertex_data	m_vert_data;
		index_buffer16				m_index_data;
		
		render_res_ptr<nrender_static_mesh_indexed>
									m_chunk_mesh;
		render_res_ptr<nrender_heightmap>
									m_heightmap_tex;
	};


	/**
	*	static mesh adapter for terrain chunk
	*/
	class nterrain_chunk_render_mesh
		: public nrender_mesh
	{
	public:
		nterrain_chunk_render_mesh(void);
		virtual ~nterrain_chunk_render_mesh(void);

		void init(nterrain_chunk* owner, nrender_static_mesh_indexed* chunk_mesh, nrender_heightmap* tex)
		{
			m_owner = owner; 
			m_chunk_mesh = chunk_mesh;
			m_heightmap_tex = tex;
		}
		virtual nvertex_factory* get_vertex_factory()	{	return m_chunk_mesh->get_vertex_factory(); }
		virtual size_t get_num_section()	{	return m_chunk_mesh->get_num_section(); }
		virtual nrender_mesh_section* get_section(size_t section_index) {	return m_chunk_mesh->get_section(section_index);}

		virtual void draw_setup_effect(nshading_effect* effect_ptr);
	private:
		nrender_static_mesh_indexed*	m_chunk_mesh;
		nrender_heightmap*				m_heightmap_tex;

		nterrain_chunk*					m_owner;
	};

	/**
	*	一个地形块作为一个primitive
	*/
	class nterrain_chunk
		: public nprimitive_component
	{
	public:
		typedef shared_ptr<nterrain_chunk> ptr;

		nterrain_chunk(const nstring& name_str);
		virtual ~nterrain_chunk(void);

		void create(const npoint& map_bias, nchunked_terrain_impl* terrain);

		virtual nrender_mesh* get_render_mesh(int lod)
		{
			(void)lod;
			return &m_mesh;
		}
		virtual nmaterial_base* get_material(int lod, int mtl_id)
		{
			(void)lod;
			(void)mtl_id;
			return m_terrain->get_material();
		}
		virtual transparent_flag get_transparent_flag() const
		{
			return m_trans_flag;
		}

		const vector3& get_map_bias() const	{	return m_map_bias;}

	private:
		nchunked_terrain_impl*		m_terrain;
		nterrain_chunk_render_mesh	m_mesh;
		transparent_flag			m_trans_flag;
		
		vector3	m_map_bias;

		nDECLARE_NAMED_CLASS(nterrain_chunk)
	};
}//namespace nexus
#endif //_NEXUS_CHUNKED_TERRAIN_IMPL_H_