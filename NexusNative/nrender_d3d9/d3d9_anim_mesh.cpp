#include "StdAfx.h"
#include "d3d9_anim_mesh.h"
#include "shading_effect.h"
#include "mesh_util.h"

namespace nexus
{
	d3d9_anim_mesh_indexed::d3d9_anim_mesh_indexed(void)
	{
	}

	d3d9_anim_mesh_indexed::~d3d9_anim_mesh_indexed(void)
	{
	}

	void d3d9_anim_mesh_indexed::create(nanim_mesh* mesh_ptr, const struct anim_mesh_morph& anim)
	{
		nASSERT( mesh_ptr != NULL );
		nASSERT( mesh_ptr->m_sequence_array.size() > 0 );		

		d3d9_vertex_factory_morph::ptr vert(nNew d3d9_vertex_factory_morph);
		vert->create(anim.src_frame_vert.get());

		_create_mesh_section_array(mesh_ptr->m_secton_array, m_section_array, D3DPOOL_DEFAULT);
		
		//-- exception safe copy
		m_vertex_factory = vert;
	}

	nrender_mesh_section* d3d9_anim_mesh_indexed::get_section(size_t section_index)
	{
		nASSERT(section_index < m_section_array.size());
		return m_section_array[section_index].get();
	}

	bool d3d9_anim_mesh_indexed::pre_render()
	{
		m_vertex_factory->prepare_frame_data(m_anim_status);
		return true;
	}

	void d3d9_anim_mesh_indexed::post_render()
	{
		m_vertex_factory->release_frame_data();
	}

	void d3d9_anim_mesh_indexed::draw_setup_effect(nshading_effect* effect_ptr)
	{
		d3d9_shading_effect* eft = static_cast<d3d9_shading_effect*>(effect_ptr);
		vector2 param(m_anim_status.alpha, 1.0f-m_anim_status.alpha);
		HRESULT hr = eft->set_float_array_by_semantic("ANIM_MorphAlpha", param);
	}
}//namespace nexus