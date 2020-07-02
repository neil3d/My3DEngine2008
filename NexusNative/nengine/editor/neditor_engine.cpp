#include "StdAfx.h"
#include "neditor_engine.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(neditor_cmd, nobject)
	nDEFINE_VIRTUAL_CLASS(neditor_engine, nengine)

	neditor_engine::neditor_engine(void)
	{
	}

	neditor_engine::~neditor_engine(void)
	{
	}	

	neditor_engine* neditor_engine::instance()
	{
		return ndynamic_cast<neditor_engine>(nengine::instance());
	}

	void neditor_engine::render_level(const nstring& lv_name, const nviewport& view, bool present)
	{
		if( !m_renderer )
			return;

		//-- render
		
			for(st_level_list::iterator iter= m_level_list.begin();
				iter != m_level_list.end();
				++iter)
			{
				nlevel::ptr lv_ptr = (*iter);
				if( lv_ptr->get_name() == lv_name )
				{
					lv_ptr->render(view);
					break;
				}
			}
	
		if(present)
			m_renderer->present(view.handle_wnd);
	}

	void neditor_engine::update_level(const nstring& lv_name, float delta_time, const nviewport& view)
	{
		nresource_manager::instance()->dispatch_events();

		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv_ptr = (*iter);
			if( lv_ptr->get_name() == lv_name )
			{
				lv_ptr->update(delta_time,view);
				break;
			}
		}
	}

	nactor::ptr neditor_engine::pick_main_level_actor(const nviewport& view, const nstring& lv_name, int mouse_x, int mouse_y)
	{
		nactor::ptr ret;		
		if(!view.hit_hash)
			return ret;

		hit_id pick_id = view.get_hit_id(mouse_x, mouse_y);		

		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv_ptr = (*iter);
			if( lv_ptr->get_name() == lv_name )
			{
				ret = lv_ptr->find_actor(pick_id);
				break;
			}
		}

		return ret;
	}
}//namespace nexus