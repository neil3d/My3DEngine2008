/**
 *	nexus ngame - nscheduler
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NSCHEDULER_H_
#define _NGAME_NSCHEDULER_H_

#include "ncommon.h"
#include "nsafe_mem_pool.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	class nscheduler : private nnoncopyable
	{
	public:
		class noperation : public OVERLAPPED
		{
		public:
			noperation()
			{
				Internal		= 0;
				InternalHigh	= 0;
				Offset			= 0;
				OffsetHigh		= 0;
				hEvent			= 0;
			}

			virtual ~noperation() {}
			virtual void do_completion() {}
		};

		template <typename Handler>
		class nhandler_operation : public noperation
		{
		public:
			nhandler_operation(Handler handler) : noperation(), m_handler(handler) {}
			virtual ~nhandler_operation() {}

			virtual void do_completion()
			{
				m_handler();
			}

		private:
			Handler		m_handler;
		};

	public:
		nscheduler();
		virtual ~nscheduler();

		bool startup();
		void shutdown();

		/**
		 * 投递一个并行执行任务，该任务在工作线程中执行
		 * NOTE: !!! post和wait要在同一线程串行调用
		 */
		template <typename Handler>
		void post(Handler handler)
		{
			nhandler_operation<Handler>* opt_ptr = _construct_operation<Handler>(handler);

			if (!opt_ptr)
			{
				return;
			}

			::ResetEvent(m_event);
			::InterlockedIncrement((LPLONG)&m_nocompleted_opt);
			::PostQueuedCompletionStatus(m_completion_port, 0, 0, LPOVERLAPPED(opt_ptr));
		}

		/**
		 * wait等待上次post的所有任务完成
		 */
		void wait()
		{
			if (::InterlockedCompareExchange((LPLONG)&m_nocompleted_opt, 0, 0) != 0)
			{
				::WaitForSingleObject(m_event, INFINITE);
			}
		}

	private:
		void _worker_thread();

		template <typename Handler>
		nhandler_operation<Handler>* _construct_operation(Handler handler)
		{
			nhandler_operation<Handler>* const opt_ptr = reinterpret_cast<nhandler_operation<Handler>* const>(m_pool.alloc(sizeof(nhandler_operation<Handler>)));

			if (NULL == opt_ptr)
			{
				return opt_ptr;
			}

			new (opt_ptr)nhandler_operation<Handler>(handler);
			return opt_ptr;
		}

		void _distory_operation(noperation* opt_ptr)
		{
			opt_ptr->~noperation();
			m_pool.free(opt_ptr);
		}

	private:
		HANDLE					m_completion_port;
		HANDLE					m_event;
		volatile LONG			m_nocompleted_opt;
		nsafe_mem_pool			m_pool;
		boost::thread_group		m_threads;
	};

} // namespace nexus

#endif // _NGAME_NSCHEDULER_H_
