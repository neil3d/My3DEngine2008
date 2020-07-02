#ifndef _NWORLD_H_
#define _NWORLD_H_

#include "nmsg_queue.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	struct nmsg_base;

	class nworld : private nnoncopyable
	{
	public:
		nworld();
		virtual ~nworld();

		bool init(uint32 max_tick_msg);
		void destroy();

		bool push_msg(nmessage* msg_ptr) { return m_msg_queue.enqueue(msg_ptr); }

		void _handle_move_test(const nmsg_base* packet);
		void _handle_load_test(const nmsg_base* packet);
		void _handle_cmd_test(const nmsg_base* packet);

	private:
		void _thread_update();
		void _handle_msg();

		void _handle_create_session(const nmsg_base* packet);
		void _handle_destory_session(const nmsg_base* packet);

		void _handle_chat_test(const nmsg_base* packet);


	private:
		boost::thread_group			m_threads;					// 线程池
		bool						m_terminate_update;
		nmsg_queue					m_msg_queue;

		nmessage*					m_cur_head_ptr;
		uint32						m_max_tick_msg;

		bool						m_tag;						// first tag
		uint32						m_session_id;				// 测试session关闭后异步调用send
	};

	extern nworld g_nworld;

} // namespace nexus

#endif //_NWORLD_H_