#include "StdAfx.h"
#include "nactor.h"
#include "ncore.h"
#include "../scene/scene_event.h"
#include "../framework/nlevel.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nactor, nevent_target)
	nDEFINE_VIRTUAL_CLASS(nactor_controller, nobject)

	nactor::nactor(const nstring& name_str):m_name(name_str)
	{
		m_owner = NULL;

		m_visible = true;		
		m_static = false;		
		m_serializable = true;
		m_frozen = false;
	}

	nactor::~nactor(void)
	{
	}

	void nactor::update(float delta_time, const nviewport& view)
	{
		if(m_static)
			return;

		if(m_controller)
			m_controller->update_actor(delta_time);

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->update(delta_time, view);
		}
	}

	nactor_component::ptr nactor::create_component(const nstring& component_name, const nstring& class_name)
	{
		nactor_component::ptr new_comp( nconstruct<nactor_component>(class_name, component_name) );
		new_comp->_init(this);

		m_component_list.push_back( new_comp );
		
		return new_comp;
	}

	void nactor::on_event(nevent_base& evt)
	{
		if( evt.m_event_id == NSCENE_EVENT_ID )
		{
			m_owner->on_event(evt);
		}
	}

	void nactor::_init(nlevel* level_ptr)
	{
		m_owner = level_ptr;
	}

	void nactor::move(const vector3& pos, const vector3& rot, const vector3& scale)
	{
		m_space.location = pos;
		m_space.rotation = rot;
		m_space.scale = scale;
		m_space.update_matrix();

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_update_transform(m_space);
		}
	}

	void nactor::_remove_from_level()
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_remove_from_level();
		}
	}

	void nactor::_destroy()
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_destroy();
		}
	}

	void nactor::serialize(narchive& ar)
	{
		nevent_target::serialize(ar);

		nSERIALIZE(ar, m_name);
		nSERIALIZE(ar, m_component_list);
		nSERIALIZE(ar, m_visible);
		nSERIALIZE(ar, m_static);
		nSERIALIZE(ar, m_space);
		nSERIALIZE(ar, m_frozen);
	}

	void nactor::_level_loaded(nlevel* level_ptr)
	{
		m_owner = level_ptr;

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_level_loaded(this);
		}
	}

	void nactor::draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->draw_bounds(PDI, color);
		}
	}

	nactor_controller::ptr nactor::create_controller(const nstring& class_name)
	{
		nactor_controller::ptr ctrl( nconstruct<nactor_controller>(class_name) );
		ctrl->_init(this);

		m_controller = ctrl;
		return ctrl;
	}
}//namespace nexus