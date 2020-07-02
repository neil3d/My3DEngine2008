#include "StdAfx.h"
#include "nprimitive_component.h"
#include "../scene/scene_event.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nprimitive_component, nactor_component)

	nprimitive_component::nprimitive_component(const nstring& name_str):nactor_component(name_str),
		m_accept_dynamic_light(true),m_cast_dynamic_shadow(true)
	{		
		mat_set_identity(m_mat_world);		

		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
	}

	nprimitive_component::~nprimitive_component(void)
	{
	}

	const matrix44& nprimitive_component::get_world_matrix() const			
	{	
		return m_mat_world;
	}		

	void nprimitive_component::_init(nactor* owner)
	{
		nactor_component::_init(owner);

		nscene_event evt(ESA_AddPrimitive, this);
		owner->on_event(evt);
	}

	void nprimitive_component::_remove_from_level()
	{
		nscene_event evt(ESA_RemovePrimitive, this);
		m_owner->on_event(evt);
	}

	void nprimitive_component::_update_transform(const object_space& parent_space)
	{
		m_mat_world = m_space.local_to_world*parent_space.local_to_world;
	}

	void nprimitive_component::serialize(narchive& ar)
	{
		nactor_component::serialize(ar);

		nSERIALIZE(ar, m_space);
		nSERIALIZE(ar, m_bounds);
		nSERIALIZE(ar, m_accept_dynamic_light);
		nSERIALIZE(ar, m_cast_dynamic_shadow);
	}

	void nprimitive_component::_level_loaded(nactor* owner)
	{
		nactor_component::_level_loaded(owner);
		this->_update_transform(owner->get_space());
		this->_init(owner);
	}

	void nprimitive_component::draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		PDI->draw_wired_box(get_AABBox(), color);
	}

	void nprimitive_component::move(const vector3& pos, const vector3& rot, const vector3& scale)
	{
		m_space.location = pos;
		m_space.rotation = rot;
		m_space.scale = scale;
		m_space.update_matrix();

		if (m_owner)
		{
			_update_transform(m_owner->get_space());
		}
		else
		{
			m_mat_world = m_space.local_to_world;
		}
	}
}//namespace nexus