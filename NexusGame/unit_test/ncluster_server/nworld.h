#ifndef _NWORLD_H_
#define _NWORLD_H_

#include "nmsg_queue.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	class nworld
	{
	public:
		nworld();
		virtual ~nworld() {}

		bool init();
		void destroy();

		bool push_msg(nmessage* msg_ptr) { return m_msg_queue.enqueue(msg_ptr); }

	private:
		void _thread_update();
		void _handle_msg();

	private:
		boost::thread_group			m_threads;					// 线程池
		bool						m_terminate_update;
		nmsg_queue					m_msg_queue;
		nmessage*					m_cur_head_ptr;
		uint32						m_proc_msg_tick;

		bool						m_tag;						// first tag
		uint32						m_session_id;				// 测试session关闭后异步调用send
	};

	extern nworld g_nworld;

} // namespace nexus

#endif //_NWORLD_H_