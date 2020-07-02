#include "StdAfx.h"
#include "nactor.h"
#include "../scene/scene_event.h"
#include "../framework/nlevel.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nactor, nobject)

	nactor::nactor(const nstring& name_str):m_name(name_str)
	{
		m_owner = NULL;

		m_visible = true;		
		m_static = false;
		mat_set_identity(m_mat_world);
	}

	nactor::~nactor(void)
	{
	}

	void nactor::update_component(float delta_time, const view_info& view)
	{
		if(m_static)
			return;

		for(component_list::iterator iter = m_component_list.begin();
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

	void nactor::move(const matrix44& mat)
	{
		m_mat_world = mat;

		for(component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_on_actor_move();
		}
	}

	void nactor::_remove_from_level()
	{
		for(component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_remove_from_level();
		}
	}
}//namespace nexus