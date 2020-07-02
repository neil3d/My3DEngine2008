#include "StdAfx.h"
#include "neditor_engine.h"

namespace nexus
{
	nDEFINE_CLASS(neditor_engine, nengine)

	neditor_engine::neditor_engine(void)
	{
	}

	neditor_engine::~neditor_engine(void)
	{
	}	

	void neditor_engine::render_level(const nstring& lv_name, const nviewport& view, bool present)
	{
		if( !m_renderer )
			return;

		//-- make view info
		m_cur_view = view;		

		//-- render
		if( m_renderer->frame_begin(view) )
		{
			for(st_level_list::iterator iter= m_level_list.begin();
				iter != m_level_list.end();
				++iter)
			{
				nlevel::ptr lv_ptr = (*iter);
				if( lv_ptr->get_name() == lv_name )
				{
					lv_ptr->render(m_cur_view, m_cur_view.hit_hash);
					break;
				}
			}

			//--
			m_renderer->frame_end();
			if(present)
				m_renderer->present(view.handle_wnd);
		}
	}

	void neditor_engine::update_level(const nstring& lv_name, float delta_time)
	{
		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv_ptr = (*iter);
			if( lv_ptr->get_name() == lv_name )
			{
				lv_ptr->update(delta_time, m_cur_view);
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