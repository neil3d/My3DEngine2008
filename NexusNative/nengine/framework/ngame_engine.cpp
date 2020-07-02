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

	void ngame_engine::frame_tick(float delta_time, const nviewport& view)
	{
		if( !m_renderer )
			return;

		m_cur_view = view;		

		if( m_renderer->frame_begin(view) )
		{
			render(m_cur_view);

			m_renderer->frame_end();

			update(delta_time);

			m_renderer->present(view.handle_wnd);
		}
	}
}//namespace nexus