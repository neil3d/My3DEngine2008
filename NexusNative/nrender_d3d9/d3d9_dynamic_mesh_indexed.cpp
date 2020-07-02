#include "StdAfx.h"
#include "d3d9_dynamic_mesh_indexed.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_dynamic_mesh_indexed, nrender_dynamic_mesh_indexed)

	d3d9_dynamic_mesh_indexed::d3d9_dynamic_mesh_indexed(void)
	{
	}

	d3d9_dynamic_mesh_indexed::~d3d9_dynamic_mesh_indexed(void)
	{
	}

	void d3d9_dynamic_mesh_indexed::create(enum EPrimitiveType prim_type,
		const nmesh_vertex_data* vert_data, const index_buffer16* index_data, int mtl_id)
	{
		nASSERT( vert_data );
		nASSERT( index_data );
		nASSERT( index_data->get_index_count() > 0);

		if( vert_data==NULL
			|| index_data==NULL
			|| index_data->get_index_count() <= 0)
			return;

		//-- create RHI resource
		d3d9_vertex_factory_common::ptr vert(nNew d3d9_vertex_factory_common);
		vert->create(vert_data, D3DPOOL_MANAGED);

		d3d9_mesh_section::ptr new_sec(nNew d3d9_mesh_section);
		new_sec->create(index_data, mtl_id, prim_type, D3DPOOL_MANAGED);

		//--
		m_vert = vert;
		m_mesh_section = new_sec;
	}

	void d3d9_dynamic_mesh_indexed::update_vertices(const nmesh_vertex_data* vert_data)
	{
		nASSERT( vert_data );
		nASSERT( m_vert );

		m_vert->update(vert_data);
	}

	void d3d9_dynamic_mesh_indexed::update_indices(const index_buffer16* index_data, size_t valid_count)
	{
		nASSERT( index_data );
		nASSERT( m_mesh_section );

		m_mesh_section->update_index(index_data, valid_count);
	}
}//namespace nexus