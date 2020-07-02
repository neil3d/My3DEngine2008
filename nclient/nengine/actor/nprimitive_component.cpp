#include "StdAfx.h"
#include "nprimitive_component.h"
#include "../scene/scene_event.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nprimitive_component, nactor_component)

	nprimitive_component::nprimitive_component(const nstring& name_str):nactor_component(name_str)
	{
		mat_set_identity(m_mat_local);
		mat_set_identity(m_mat_world);		
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

	void nprimitive_component::_on_actor_move()
	{
		m_mat_world = m_mat_local * m_owner->get_world_matrix();
	}

}//namespace nexus