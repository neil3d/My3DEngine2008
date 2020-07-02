/**
 *	nexus ngame - ntimer
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NTIMER_H_
#define _NGAME_NTIMER_H_

#include "ncommon.h"

namespace nexus {

	class ninterval_timer
	{
	public:
		ninterval_timer() : m_interval(0), m_current(0) {}
		virtual ~ninterval_timer() {}

		void update(uint32 elapse)
		{
			m_current += elapse;
		}

		bool passed() const
		{
			return m_current >= m_interval;
		}

		void reset()
		{
			while (m_current >= m_interval)
			{
				m_current -= m_interval;
			}
		}

		void set_current(uint32 current)	{ m_current = current; }
		void set_interval(uint32 interval)	{ m_interval = interval; }
		uint32 get_interval() const			{ return m_interval; }
		uint32 get_current() const			{ return m_current; }

	private:
		uint32					m_interval;
		uint32					m_current;
	};

	// 注意并发任务访问时的线程安全
	class ngame_timer : private nnoncopyable
	{
	public:
		ngame_timer();
		virtual ~ngame_timer();

		bool init();
		void destroy();
		void update(uint32 elapse);

		time_t get_start_time() const		{ return m_start_time; }
		time_t get_current_time() const		{ return m_game_time; }

		float get_time_stamp() const		{ return m_time_stamp; }

	private:
		float					m_time_stamp;
		time_t					m_start_time;
		time_t					m_game_time;
	};

	extern ngame_timer g_game_timer;

} // namespace nexus

#endif // _NGAME_NTIMER_H_
