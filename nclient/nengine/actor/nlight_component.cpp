#include "StdAfx.h"
#include "nlight_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nlight_component, nactor_component)

		nlight_component::nlight_component(const nstring& name_str):nactor_component(name_str)
	{
	}

	nlight_component::~nlight_component(void)
	{
	}

	void nlight_component::create_point_light()
	{
		m_light_proxy.reset( nNew npoint_light_proxy(this) );
	}

	npoint_light_proxy* nlight_component::get_point_light()
	{
		if( m_light_proxy )
		{
			nrender_light_proxy* lgt_base = m_light_proxy.get();
			if(lgt_base->m_type == ELight_Point)
				return static_cast<npoint_light_proxy*>(lgt_base);
		}

		return NULL;
	}

	void nlight_component::render(const view_info& view)
	{
		(void)view;

		if( m_light_proxy )
		{
			nrenderer_base* rnd = nengine::instance()->get_renderer();
			rnd->frame_light(m_light_proxy.get());
		}
	}

	void nlight_component::create_directional_light()
	{
		m_light_proxy.reset( nNew ndirectional_light_proxy(this) );
	}

	ndirectional_light_proxy* nlight_component::get_directional_light()
	{
		if( m_light_proxy )
		{
			nrender_light_proxy* lgt_base = m_light_proxy.get();
			if(lgt_base->m_type == ELight_Directional)
				return static_cast<ndirectional_light_proxy*>(lgt_base);
		}

		return NULL;
	}

	void nlight_component::update(float delta_time, const view_info& view)
	{
		(void)view;

		if( !m_light_proxy )
			return;

		switch(m_light_proxy->m_type)
		{
		case ELight_Point:
			{
				vector3 zero(0,0,0);//todo:…Ë÷√lgith pos£ø
				npoint_light_proxy* lgt = static_cast<npoint_light_proxy*>(m_light_proxy.get());
				lgt->m_position = zero*m_owner->get_world_matrix();
			}
			break;
		case ELight_Directional:
			break;
		}
	}

	void nlight_component::_init(nactor* owner)
	{
		nactor_component::_init(owner);

		nscene_event evt(ESA_AddLight, this);
		owner->on_event(evt);
	}

	void nlight_component::_on_actor_move()
	{}

	void nlight_component::_remove_from_level()
	{
		nscene_event evt(ESA_RemoveLight, this);
		m_owner->on_event(evt);
	}
}//namespace nexus