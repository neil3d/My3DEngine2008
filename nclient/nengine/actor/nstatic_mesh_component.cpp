#include "StdAfx.h"
#include "nstatic_mesh_component.h"
#include "../framework/nengine.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nstatic_mesh_component, nmesh_component)

	nstatic_mesh_component::nstatic_mesh_component(const nstring& name_str)
		:nmesh_component(name_str)
	{
	}

	nstatic_mesh_component::~nstatic_mesh_component(void)
	{		
	}

	void nstatic_mesh_component::reset_resource(nresource_static_mesh::ptr mesh_ptr)
	{
		m_static_mesh = mesh_ptr;

		m_bounds = m_static_mesh->get_bounds();
		m_bounds.transform_by( get_world_matrix() );
	}

	nrender_mesh* nstatic_mesh_component::get_render_mesh(int lod)
	{
		return m_static_mesh->get_render_mesh(lod);
	}

	nmaterial_base* nstatic_mesh_component::get_material(int lod, int mtl_id)
	{
		nmaterial_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		return m_static_mesh->get_material(lod, mtl_id);
	}

	void nstatic_mesh_component::_on_actor_move()
	{
		nmesh_component::_on_actor_move();

		m_bounds = m_static_mesh->get_bounds();
		m_bounds.transform_by( get_world_matrix() );

		nscene_event evt(ESA_MovePrimitive, this);
		m_owner->on_event(evt);
	}

	void nstatic_mesh_component::draw_simple_mesh(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		PDI->draw_simple_mesh( m_static_mesh->get_render_simple_mesh(), color );
	}

}//namespace nexus