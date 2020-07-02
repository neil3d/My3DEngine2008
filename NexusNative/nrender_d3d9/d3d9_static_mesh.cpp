#include "StdAfx.h"
#include "d3d9_static_mesh.h"
#include "mesh_func.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_static_mesh, nrender_static_mesh)

	d3d9_static_mesh::d3d9_static_mesh(void) 
	{
	}

	d3d9_static_mesh::~d3d9_static_mesh(void)
	{
	}

	void d3d9_static_mesh::create(enum EPrimitiveType prim_type, const nmesh_vertex_data* vb)
	{
		d3d9_vertex_factory_common::ptr vert(nNew d3d9_vertex_factory_common);
		vert->create(vb, D3DPOOL_DEFAULT);
		
		//--
		m_vert = vert;
		m_section.create_none_index(0, prim_type,
			get_primitive_count_by_vert(prim_type, vb->get_num_vert())
			);
	}

}//namespace nexus