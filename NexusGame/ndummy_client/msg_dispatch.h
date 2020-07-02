#pragma once

#include "ncommon.h"
#include "msg_queue.h"
#include "client_define.h"

class msg_dispatch
{
public:
	msg_dispatch();
	virtual ~msg_dispatch();

	bool init();
	void update(uint32 elapse);
	void destroy();

	bool push_msg(const void* msg_ptr, uint32 size)
	{
		m_recv_total_sec += (size + 2);
		return m_msg_queue.enqueue(msg_ptr, size);
	}

	uint32 get_recv_sec() const { return m_recv_sec; }

private:
	void _register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler);
	void _handle_msg(client_msg* msg_ptr);

private:
	uint32						m_recv_total_sec;
	uint32						m_recv_sec;
	uint32						m_time_stamp;

	msg_queue					m_msg_queue;
	client_msg_handler			m_client_msg_handler[Msg_Max];
};

extern msg_dispatch g_msg_dispatch;
