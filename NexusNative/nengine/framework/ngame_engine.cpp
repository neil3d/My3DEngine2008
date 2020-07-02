#include "StdAfx.h"
#include "ngame_engine.h"
#include "../image/nimage.h"

namespace nexus
{
	nDEFINE_CLASS(ngame_engine, nengine)
	ngame_engine::ngame_engine(void)
	{
	}

	ngame_engine::~ngame_engine(void)
	{
	}

	void ngame_engine::update(float delta_time, const nviewport& view)
	{
		nengine::update(delta_time,view);		
	}

	void ngame_engine::render( const nviewport& view )
	{
		nengine::render(view);
	}

	void ngame_engine::screen_shot( const nstring& file_name, size_t width, size_t height )
	{
		if( m_renderer != NULL )
		{
			try
			{
				nimage img;
				nengine::instance()->get_renderer()->screen_shot(&img, width, height);
				img.save_to_file(file_name);

				nLog_Info( _T("screen shot [%s]...\r\n"), file_name.c_str());
			}
			catch (...)
			{
				nLog_Info( _T("screen shot faild [%s]...\r\n"), file_name.c_str());
			}
		}
	}

	void ngame_engine::viewport_shot( const nstring& file_name )
	{
		if( m_renderer != NULL )
		{
			try
			{
				nimage img;
				nengine::instance()->get_renderer()->viewport_shot(&img);
				img.save_to_file(file_name);

				nLog_Info( _T("screen shot [%s]...\r\n"), file_name.c_str());
			}
			catch (...)
			{
				nLog_Info( _T("screen shot faild [%s]...\r\n"), file_name.c_str());
			}
		}
	}

	nactor::ptr ngame_engine::pick_level_actor( const nviewport& view, int mouse_x, int mouse_y )
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
			ret = lv_ptr->find_actor(pick_id);
			if(ret)
			{
				break;
			}
		}

		return ret;
	}
}//namespace nexus