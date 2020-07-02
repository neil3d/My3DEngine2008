/**
 *	nexus nperfmon_test - nperfmon_test
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NPERFMON_TEST_H_
#define _NPERFMON_TEST_NPERFMON_TEST_H_

#include "nsingleton.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	压力测试客户端主循环
	 */
	class nperfmon_test : public nsingleton<nperfmon_test>
	{
	private:
		friend class nsingleton<nperfmon_test>;

	protected:
		nperfmon_test();

	public:
		~nperfmon_test();

		bool init();
		void destroy();
		bool run();
		static void stop();

		float get_time_stamp() const			{ return m_time_stamp; }
		void set_time_stamp(float time_stamp)	{ m_time_stamp = time_stamp; }

	private:
		bool _init_game();
		void _update_thread();
		void _cmd_line_thread();

		static void _on_signal(int s);
		void _hook_signals();
		void _unhook_signals();

	private:
		boost::thread_group						m_threads;
		static volatile LONG					m_terminate;
		float									m_time_stamp;
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NPERFMON_TEST_H_
