/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_MOUSE_HELPER_H_
#define _NEXUS_MOUSE_HELPER_H_
#include <boost/timer.hpp>

namespace nexus
{
	/**
	 * 鼠标拖动管理
	 * @brief 主要用来辅助控制拖动中的操作重复间隔
	*/
	struct mouse_drag
	{
		bool			m_dragging;
		boost::timer	m_timer;
		double			m_repeat_time;

		mouse_drag():m_dragging(false),m_repeat_time(0.1)
		{}

		/** 是否处于拖动状态 */
		bool draging() const	{	return m_dragging; }

		/** 鼠标键按下时调用 */
		void begin_drag()
		{
			m_timer.restart();
			m_dragging = true;
		}

		/** 鼠标键抬起时调用 */
		void end_drag()
		{
			m_dragging = false;
		}

		/** 是否应该产生重复操作 */
		bool repeat()
		{
			if( !m_dragging )
				return false;

			if( m_timer.elapsed() > m_repeat_time )
			{
				m_timer.restart();
				return true;
			}
			else
				return false;
		}
	};

}//namespace nexus
#endif