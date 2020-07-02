/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_STATIC_MESH_INDEXED_H_
#define _NEXUS_D3D9_STATIC_MESH_INDEXED_H_
#include "d3d9_vertex_factory_common.h"
#include "d3d9_mesh_section.h"

namespace nexus
{
	class d3d9_static_mesh_indexed 
		: public nrender_static_mesh_indexed
	{
	public:
		d3d9_static_mesh_indexed(void);
		~d3d9_static_mesh_indexed(void);

		virtual nvertex_factory* get_vertex_factory()		{	return m_vert.get();}
		virtual size_t get_num_section()					{	return m_section_array.size();}		
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			nASSERT(section_index < m_section_array.size());
			return m_section_array[section_index].get();
		}

		virtual void create(nstatic_mesh_lod* mesh);
		virtual void create(enum EPrimitiveType prim_type,
			const nmesh_vertex_data* vert_data, const index_buffer16* index_data, int mtl_id);
		virtual size_t append_section(enum EPrimitiveType prim_type, const index_buffer16* index_data, int mtl_id);

		// 静态mesh也提供update内部buffer的接口，但是只适合更新频率极低的情况，否则会有较大的性能损失
		virtual void update_vertex_buffer(const nmesh_vertex_data* vert_data);

	private:		
		d3d9_vertex_factory_common::ptr		m_vert;		
		vector<d3d9_mesh_section::ptr>		m_section_array;		

		nDECLARE_CLASS(d3d9_static_mesh_indexed)
	};
}//namespace nexus
#endif //_NEXUS_D3D9_STATIC_MESH_INDEXED_H_