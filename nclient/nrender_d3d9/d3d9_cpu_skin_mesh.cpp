#include "StdAfx.h"
#include "d3d9_cpu_skin_mesh.h"
#include "mesh_util.h"

namespace nexus
{
	d3d9_cpu_skin_mesh::nshader_modifier_cpu_skin d3d9_cpu_skin_mesh::s_shader_modifier;

	d3d9_cpu_skin_mesh::d3d9_cpu_skin_mesh(void)
	{
	}

	d3d9_cpu_skin_mesh::~d3d9_cpu_skin_mesh(void)
	{
	}

	void d3d9_cpu_skin_mesh::create(nskeletal_mesh::ptr mesh_ptr)
	{		
		nASSERT( mesh_ptr );

		nrender_cpu_skin_mesh::create(mesh_ptr);

		d3d9_vertex_factory_dynamic::ptr vert(nNew d3d9_vertex_factory_dynamic);
		vert->create(mesh_ptr->m_vert_data.get());

		_create_mesh_section_array(mesh_ptr->m_secton_array, m_section_array, D3DPOOL_DEFAULT);

		//--
		m_vertex_factory = vert;
	}

	nvertex_factory* d3d9_cpu_skin_mesh::get_vertex_factory()
	{
		return m_vertex_factory.get();
	}

	size_t d3d9_cpu_skin_mesh::get_num_section()
	{
		return m_section_array.size();
	}

	nrender_mesh_section* d3d9_cpu_skin_mesh::get_section(size_t section_index)
	{
		nASSERT(section_index < m_section_array.size());
		return m_section_array[section_index].get();
	}

	bool d3d9_cpu_skin_mesh::pre_render()
	{
		m_vertex_factory->prepare_frame_data( m_skined_vert_data.get() );
		return true;
	}

	void d3d9_cpu_skin_mesh::post_render()
	{
		m_vertex_factory->release_frame_data();
	}
}//namespace nexus