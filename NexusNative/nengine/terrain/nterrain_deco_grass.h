#ifndef _NEXUS_TERRAIN_DECO_GRASS_H_
#define _NEXUS_TERRAIN_DECO_GRASS_H_

#include "nterrain_deco_chunk_base.h"
#include "../mesh/mesh_lod.h"
#include "../renderer/nrender_static_mesh.h"
#include "../mesh/mesh_vertex_data.h"

//草叶顶点和网格数据的定义

namespace nexus
{
	//草叶顶点数据
	class nAPI nmesh_vertex_data_grass
		: public nmesh_vertex_data
	{
	public:
		typedef shared_ptr<nmesh_vertex_data_grass> ptr;

		nmesh_vertex_data_grass(void);
		~nmesh_vertex_data_grass(void);

		void	init(size_t num_vert);	

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		virtual size_t get_num_stream() const	{	return 2;}
		virtual vertex_stream* get_stream(size_t i)
		{
			nASSERT(i < 2);
			vertex_stream* ptr_array[3] = 
			{	&m_pos_stream,
				&m_second_stream,	
				0,
			};

			return ptr_array[i];
		}

		virtual bool support_pos_only() const	{	return false;}
	protected:
		/*struct vert_part
		{
			vector3		normal;
			vector2		uv;
			float		weight;
		};*/
		
		vertex_stream	m_second_stream;
	};

	//疑问：是为每一株草叶生成12个顶点+18个顶点索引，还是使用instance计数，为每一株草叶生成一个4x4矩阵？
	class nAPI grass_deco_buffer
	{
		struct second_part 
		{
			vector3		normal;
			vector2		uv;
			float		weight;
		};
	public:
		typedef boost::shared_ptr<grass_deco_buffer>	ptr;
		void	set_material(nmtl_base::ptr matrial);
		void	set_cross(bool value)	{m_cross=value;}
		void	begin_update();
		void	add_instance(const matrix44& matrix);
		void	end_update();
		void	get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp);
		void	on_device_lost();
		void	on_device_reset();

	protected:
		//用于渲染的草叶的顶点数据
		nmesh_vertex_data_grass::ptr	m_vertex;
		index_buffer16			m_index;
		nrender_mesh_section	m_section;
		nmtl_base::ptr			m_material;
		render_res_ptr<nrender_static_mesh>	m_render_mesh;
		bool					m_cross;
		//-----------------------------------------------------------------------------------
		vector3 m_pos[12];
		vector<vector3>	m_pos_stream;
		second_part	m_second_part[12];
		vector<second_part>	m_second_stream;
	public:
		grass_deco_buffer();
		~grass_deco_buffer();
	};
};

#endif