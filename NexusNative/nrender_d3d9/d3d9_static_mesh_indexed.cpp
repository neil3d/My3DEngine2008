#include "StdAfx.h"
#include "d3d9_static_mesh_indexed.h"
#include "mesh_func.h"
#include "mesh_util.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_static_mesh_indexed, nrender_static_mesh_indexed)

	d3d9_static_mesh_indexed::d3d9_static_mesh_indexed(void)
	{		
	}

	d3d9_static_mesh_indexed::~d3d9_static_mesh_indexed(void)
	{
	}

	void d3d9_static_mesh_indexed::create(nstatic_mesh_lod* mesh)
	{
		nASSERT(mesh != NULL);
		nASSERT(mesh->m_secton_array.size() > 0);

		if(mesh == NULL
			|| mesh->m_secton_array.size() <= 0)
			return;

		//-- create RHI resource
		d3d9_vertex_factory_common::ptr vert(nNew d3d9_vertex_factory_common);
		vert->create(mesh->m_vert_data.get(), D3DPOOL_DEFAULT);

		//-- mesh section
		_create_mesh_section_array(mesh->m_secton_array, m_section_array, D3DPOOL_DEFAULT);

		//-- exception safe copy
		m_vert = vert;
	}

	void d3d9_static_mesh_indexed::create(enum EPrimitiveType prim_type,
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
		vert->create(vert_data, D3DPOOL_DEFAULT);

		try
		{
			d3d9_mesh_section::ptr new_sec(nNew d3d9_mesh_section);
			new_sec->create(index_data, mtl_id, prim_type, D3DPOOL_DEFAULT);

			m_section_array.push_back(new_sec);			
		}
		catch(nexception& e)
		{
			(void)e;

			//-- for exception safe
			m_section_array.clear();

			//-- Òì³£ÖÐÁ¢
			throw;
		}

		//-- exception safe copy
		m_vert = vert;
	}

	size_t d3d9_static_mesh_indexed::append_section(enum EPrimitiveType prim_type, const index_buffer16* index_data, int mtl_id)
	{
		d3d9_mesh_section::ptr new_sec(nNew d3d9_mesh_section);
		new_sec->create(index_data, mtl_id, prim_type, D3DPOOL_DEFAULT);

		m_section_array.push_back(new_sec);

		return m_section_array.size()-1;
	}

	void d3d9_static_mesh_indexed::update_vertex_buffer(const nmesh_vertex_data* vert_data)
	{
		m_vert->update(vert_data);
	}
}//namespace nexus