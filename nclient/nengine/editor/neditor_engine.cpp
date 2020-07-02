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

	void neditor_engine::render_level(const nstring& lv_name, const ncamera* cam, int wnd_handle)
	{
		if( !m_renderer )
			return;

		//-- make view info
		m_cur_view.cam = cam;
		m_cur_view.frustum.extract(cam->get_view(), cam->get_project());
		m_cur_view.handle_wnd = (void*)wnd_handle;

		//-- render
		if( m_renderer->frame_begin(cam) )
		{
			for(level_list::iterator iter= m_level_list.begin();
				iter != m_level_list.end();
				++iter)
			{
				nlevel::ptr lv_ptr = (*iter);
				if( lv_ptr->get_name() == lv_name )
					lv_ptr->render(m_cur_view);
			}

			//--
			m_renderer->frame_end();
			m_renderer->present((void*)wnd_handle);
		}
	}

	void neditor_engine::update_objects(float delta_time)
	{
		update(delta_time);
	}
}//namespace nexus