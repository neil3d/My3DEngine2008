#pragma once
#include "msg_listener.h"

namespace nexus
{
	/** 消息监听器实现 log消息 */
	struct nClientNetworkAPI msg_log_listener : public msg_listener
	{
		virtual void on_msg(uint16 message_id, const nmsg_base* msg_ptr)
		{
			nASSERT(message_id != msg_ptr->msg_id);
			nLog_Info(_T("recv message, id = %u \r\n"), msg_ptr->msg_id);
		}
	};

	typedef boost::shared_ptr<msg_log_listener> msg_log_listener_ptr;
}


