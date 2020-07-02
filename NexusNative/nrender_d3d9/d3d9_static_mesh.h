/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_STATIC_MESH_H_
#define _NEXUS_D3D9_STATIC_MESH_H_
#include "d3d9_vertex_factory_common.h"
#include "d3d9_mesh_section.h"

namespace nexus
{
	class d3d9_static_mesh : public nrender_static_mesh
	{
	public:
		d3d9_static_mesh(void);
		virtual ~d3d9_static_mesh(void);

		virtual nvertex_factory* get_vertex_factory()		{	return m_vert.get();}
		virtual size_t get_num_section()					{	return 1; }
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			return &m_section;
		}

		virtual void create(enum EPrimitiveType prim_type, const nmesh_vertex_data* vb);
	
	private:
		d3d9_vertex_factory_common::ptr	m_vert;		
		d3d9_mesh_section			m_section;

		nDECLARE_CLASS(d3d9_static_mesh)
	};
}//namespace nexus
#endif //_NEXUS_D3D9_STATIC_MESH_H_