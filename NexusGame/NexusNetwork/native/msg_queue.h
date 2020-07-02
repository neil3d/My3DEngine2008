#pragma once
#include "ncommon.h"
#include "nsafe_mem_pool.h"

namespace nexus
{
	struct client_msg;

	/**
	* 消息队列
	*/
	class msg_queue
	{
	public:
		msg_queue();
		virtual ~msg_queue();

		bool enqueue(const void* data_ptr, uint32 size);
		client_msg* dequeue();
		void clear();

	private:
		nsafe_mem_pool					m_msg_pool;
		nfast_recursive_mutex			m_mutex;				// 线程安全锁

		uint32							m_max_size;				// 队列所容纳的消息最大条数
		uint32							m_size;					// 队列当前消息条数

		client_msg*						m_head_ptr;				// 用于出队
		client_msg*						m_tail_ptr;				// 用于入队
	};
}
