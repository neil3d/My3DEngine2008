/**
 *	nexus nlogin - nlogin
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_H_
#define _NLOGIN_NLOGIN_H_

#include "nsingleton.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "ncmd_queue.h"

namespace nexus {

	/**
	 *	登录服务器的主循环
	 */
	class nlogin : public nsingleton<nlogin>
	{
	private:
		friend class nsingleton<nlogin>;

	protected:
		nlogin();

	public:
		~nlogin();

		bool init();
		void destroy();
		bool run();
		static void stop();

		ncmd_queue& get_cmd_queue(void){ return m_cmd_queue; }
	private:
		void _update_thread();
		void _cmd_line_thread();

		static void _on_signal(int s);
		void _hook_signals();
		void _unhook_signals();

	private:
		static volatile LONG					m_terminate;
		boost::thread_group						m_threads;
		ncmd_queue								m_cmd_queue;
	};

} // namespace nexus

#endif // _NLOGIN_NLOGIN_H_
