/**
 *	nexus nworld - nworld
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NWORLD_NWORLD_H_
#define _NWORLD_NWORLD_H_

#include "nsingleton.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	世界服务器的主循环
	 */
	class nworld : public nsingleton<nworld>
	{
	private:
		friend class nsingleton<nworld>;

	protected:
		nworld();

	public:
		~nworld();

		bool init();
		void destroy();
		bool run();
		static void stop();

	private:
		bool _init_game();
		void _destroy_game();
		void _update_thread();

		static void _on_signal(int s);
		void _hook_signals();
		void _unhook_signals();

	private:
		boost::scoped_ptr<boost::thread>		m_update_thread_ptr;
		static volatile LONG					m_terminate;
	};

} // namespace nexus

#endif // _NWORLD_NWORLD_H_
