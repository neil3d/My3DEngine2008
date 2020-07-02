#include "StdAfx.h"
#include "nspecial_effect_instance.h"
#include "../resource/nresource_manager.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nspecial_effect_instance, nprimitive_component)

	nspecial_effect_instance::nspecial_effect_instance(const nstring& name_str):nprimitive_component(name_str)
	{
		m_accept_dynamic_light = false;
		m_cast_dynamic_shadow = false;

		m_playing = false;
		m_play_time = 0;
		m_visible=true;

		nresource_manager::instance()->register_event_target(this);
	}

	nspecial_effect_instance::~nspecial_effect_instance(void)
	{
		nresource_manager::instance()->unregister_event_target(this);
	}

	void nspecial_effect_instance::reset_resource(nresource_special_effect::ptr new_res)
	{
		//-- create new
		m_res = new_res;

		if( m_res && m_res->ready() )
			on_resource_ready();
	}

	void nspecial_effect_instance::on_resource_ready()
	{
		//--可重复调用的
		//-- clear old data
		//m_elements.clear();

		//--
		size_t num_elem = m_res->get_num_element();
		for(size_t i=0; i<num_elem; i++)
		{
			neffect_element::ptr elem_temp = m_res->get_element(i);
			neffect_element_instance::ptr elem_inst	= elem_temp->create_instance();
			elem_inst->init( elem_temp );

		/*	sfx_proxy::ptr proxy( new sfx_proxy );
			proxy->sfx_init(this, elem_inst);
			m_elements.push_back( proxy );*/
		}
	}

	void nspecial_effect_instance::update(float delta_time, const nviewport& view)
	{
		nprimitive_component::update(delta_time, view);

		if( !m_res
			|| !m_res->ready() )
			return;

		if(!m_playing)
			return;

		m_play_time += delta_time;

		//-- 计算当前的lod

		//-- 更新当前lod的所有元素
		size_t num_elem = m_res->get_num_element();
		/*nASSERT( num_elem == m_elements.size() );
		for(size_t i=0; i<num_elem; i++)
		{
			neffect_element::ptr elem_template = m_res->get_element(i);
			sfx_proxy::ptr elem_obj = m_elements[i];
			elem_template->update_instance(elem_obj->m_sfx_inst, view, delta_time, m_play_time);
		}*/
	}

	void nspecial_effect_instance::render(const nviewport& view)
	{
		if(!m_visible) return;
		nrenderer_base* rnd = nengine::instance()->get_renderer();
		
		/*	size_t num_elem = m_elements.size();
		for (size_t i=0; i<num_elem; i++)
		{
			neffect_element::ptr elem_template = m_res->get_element(i);
			if(!elem_template->is_visible()) continue;

	sfx_proxy::ptr elem_obj = m_elements[i];

			-- 更新特效元素的渲染数据
			if( elem_obj->m_sfx_inst->update_render_data(view, m_play_time, get_world_matrix()) )
			{
				-- 提交到渲染器
				rnd->frame_object(elem_obj.get());
			}
		}*/		
	}

	void nspecial_effect_instance::play()
	{
		m_play_time = 0;
		m_playing = true;
	}

	void nspecial_effect_instance::close(bool fade)
	{
		(void)fade;
		m_playing = false;
	}

	void nspecial_effect_instance::on_event(nevent_base& evt)
	{
		if( m_res
			&& is_resource_ready_event(evt, m_res->get_name() )
			)
		{
			on_resource_ready();
		}
	}
}//namespace nexus