#ifndef _NEXUS_D3D9_DYNAMIC_MESH_H_
#define _NEXUS_D3D9_DYNAMIC_MESH_H_
#include "d3d9_vertex_factory_common.h"
#include "d3d9_mesh_section.h"

namespace nexus
{
	class d3d9_dynamic_mesh
		: public nrender_dynamic_mesh
	{
	public:
		d3d9_dynamic_mesh(void);
		~d3d9_dynamic_mesh(void);

		virtual void create(enum EPrimitiveType prim_type, const nmesh_vertex_data* vert_data, int mtl_id);
		virtual void update_vertices(const nmesh_vertex_data* vert_data);		

		//-- render mesh interface
		virtual nvertex_factory* get_vertex_factory()	{	return m_vert.get();}
		virtual size_t get_num_section()	{	return 1; }	
		virtual nrender_mesh_section* get_section(size_t section_index)	{	return m_mesh_section.get();}

		virtual void set_draw_limit(int primitive_count)
		{
			nrender_dynamic_mesh::set_draw_limit(primitive_count);
			m_mesh_section->set_draw_limit(primitive_count);
		}

	private:
		d3d9_vertex_factory_common::ptr		m_vert;		
		d3d9_mesh_section::ptr				m_mesh_section;

		nDECLARE_CLASS(d3d9_dynamic_mesh)
	};

}//namespace nexus
#endif //_NEXUS_D3D9_DYNAMIC_MESH_H_