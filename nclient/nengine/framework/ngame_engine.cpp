#include "StdAfx.h"
#include "ngame_engine.h"

namespace nexus
{
	ngame_engine::ngame_engine(void)
	{
	}

	ngame_engine::~ngame_engine(void)
	{
	}

	void ngame_engine::frame_tick(float delta_time, const ncamera* cam, void* hWindow)
	{
		if( !m_renderer )
			return;

		m_cur_view.cam = cam;
		m_cur_view.frustum.extract(cam->get_view(), cam->get_project());
		m_cur_view.handle_wnd = hWindow;

		if( m_renderer->frame_begin(cam) )
		{
			render(m_cur_view);

			m_renderer->frame_end();

			update(delta_time);

			m_renderer->present(hWindow);
		}
	}
}//namespace nexus