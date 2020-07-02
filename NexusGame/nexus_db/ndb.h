/**
 *	nexus ndb - ndb
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDB_NDB_H_
#define _NDB_NDB_H_

#include "nsingleton.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "ncmd_queue.h"
namespace nexus {

	/**
	 *	DB服务器的主循环
	 */
	class ndb : public nsingleton<ndb>
	{
	private:
		friend class nsingleton<ndb>;

	protected:
		ndb();

	public:
		~ndb();

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
		static volatile LONG	m_terminate;
		boost::thread_group		m_threads;
		ncmd_queue				m_cmd_queue;
	};

} // namespace nexus

#endif // _NDB_NDB_H_
