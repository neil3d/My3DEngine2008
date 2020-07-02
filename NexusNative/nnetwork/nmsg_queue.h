/**
 *	nexus network - nmsg_queue
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NMSG_QUEUE_H_
#define _NNETWORK_NMSG_QUEUE_H_

#include "nmutex.h"

namespace nexus {

	struct nmessage;

	/**
	 *	nmsg_queue管理逻辑消息
	 *	@remark nmsg_queue 放入该队列的消息不共享，TODO: D. Zhao 丰富取消息的接口
	 */
	class nNET_API nmsg_queue : private nnoncopyable
	{
	public:
		nmsg_queue() : m_max_size(0), m_size(0), m_head_ptr(NULL), m_tail_ptr(NULL) {}
		explicit nmsg_queue(uint32 max_size) : m_max_size(max_size), m_size(0), m_head_ptr(NULL), m_tail_ptr(NULL) {}
		virtual ~nmsg_queue();

		bool enqueue(nmessage* msg_ptr);
		nmessage* dequeue();
		void clear();

		NINLINE uint32 get_msg_num() const { return m_size; }

	private:
		nfast_recursive_mutex	m_mutex;				// 线程安全锁

		uint32					m_max_size;				// 队列所容纳的消息最大条数
		uint32					m_size;					// 队列当前消息条数

		nmessage*				m_head_ptr;				// 用于出队
		nmessage*				m_tail_ptr;				// 用于入队
	};

} // namespace nexus

#endif _NNETWORK_NMSG_QUEUE_H_
