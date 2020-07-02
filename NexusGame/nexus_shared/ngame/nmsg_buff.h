/**
 *	nexus ngame - nmsg_buff
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NMSG_BUFF_H_
#define _NGAME_NMSG_BUFF_H_

#include "ncommon.h"

namespace nexus {

	struct nmessage;

	class nmsg_buff : private nnoncopyable
	{
	public:
		nmsg_buff();
		virtual ~nmsg_buff();

		bool init(uint8 max_size);

		nmessage* push_msg(nmessage* msg_ptr, bool map_task, bool& overflow);
		nmessage* pop_msg(bool map_task);
		void clear();

	private:
		nmessage*				m_head_ptr;				// 用于出队
		nmessage*				m_tail_ptr;				// 用于入队

		uint8					m_max_size;				// 队列所容纳的消息最大条数
		uint8					m_size;					// 队列当前消息条数
	};

} // namespace nexus

#endif // _NGAME_NMSG_BUFF_H_
