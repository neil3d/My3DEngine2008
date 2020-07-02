#include "StdAfx.h"
#include "d3d9_dynamic_mesh.h"
#include "mesh_func.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_dynamic_mesh, nrender_dynamic_mesh)

	d3d9_dynamic_mesh::d3d9_dynamic_mesh(void)
	{
	}

	d3d9_dynamic_mesh::~d3d9_dynamic_mesh(void)
	{
	}

	void d3d9_dynamic_mesh::create(enum EPrimitiveType prim_type, const nmesh_vertex_data* vert_data, int mtl_id)
	{
		nASSERT( vert_data );
		

		if( vert_data==NULL )
			return;

		//-- create RHI resource
		d3d9_vertex_factory_common::ptr vert(nNew d3d9_vertex_factory_common);
		vert->create(vert_data, D3DPOOL_MANAGED);

		d3d9_mesh_section::ptr new_sec(nNew d3d9_mesh_section);
		new_sec->create_none_index(mtl_id, prim_type, 
			get_primitive_count_by_vert(prim_type, vert_data->get_num_vert())
			);

		//--
		m_vert = vert;
		m_mesh_section = new_sec;
	}

	void d3d9_dynamic_mesh::update_vertices(const nmesh_vertex_data* vert_data)
	{
		nASSERT( vert_data );
		nASSERT( m_vert );

		m_vert->update(vert_data);
	}
}//namespace nexus