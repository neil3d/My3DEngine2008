/**
 *	nexus ngateway - ngateway
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NGATEWAY_H_
#define _NGATEWAY_NGATEWAY_H_

#include "nsingleton.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	网关服务器的主循环
	 */
	class ngateway : public nsingleton<ngateway>
	{
	private:
		friend class nsingleton<ngateway>;

	protected:
		ngateway();

	public:
		~ngateway();

		bool init();
		void destroy();
		bool run();
		static void stop();

	private:
		void _update_thread();

		static void _on_signal(int s);
		void _hook_signals();
		void _unhook_signals();

	private:
		boost::scoped_ptr<boost::thread>		m_update_thread_ptr;
		static volatile LONG					m_terminate;
	};

} // namespace nexus

#endif // _NGATEWAY_NGATEWAY_H_
