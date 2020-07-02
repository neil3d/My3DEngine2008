#include "ncserver.h"
#include "nmsg_queue.h"
#include "cluster_echo_test.h"
#include "nworld.h"
#include "nperfmon.h"

#include <iostream>

namespace nexus {

	ncserver::ncserver()
	{
	}

	ncserver::~ncserver()
	{
	}

	void ncserver::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		tagNCI_Connect* msg_ptr = alloc_msg<tagNCI_Connect>();

		if (!msg_ptr)
		{
			std::cout << "mem pool alloc error" << std::endl;
			return;
		}

		msg_ptr->param1		= param1;
		msg_ptr->param2		= param2;

		nmessage* _msg_ptr	= NS2MSG(msg_ptr);
		_msg_ptr->session_id	= session_id;

		// test close & close send
		// close(session_id);
		//tagNCS_Reply* rep_ptr = alloc_msg<tagNCS_Reply>();

		//if (rep_ptr)
		//{
		//	send(session_id, NS2MSG(rep_ptr), sizeof(tagNCS_Reply));
		//}

		//return;

		g_nworld.push_msg(_msg_ptr);
	}

	void ncserver::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// test send
		//msg_ptr->reference = 1;
		//send(session_id, msg_ptr, size);
		//return;

		// test close
		//send(session_id, msg_ptr, size);
		//close(session_id);
		//return;

		g_nperfmon.recv(size);

		msg_ptr->session_id = session_id;

		//const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));

		//switch (packet->msg_id)
		//{
		//case NCC_StressTestMove:
		//	{
		//		tagNCC_StressTestMove* msg_data_ptr = (tagNCC_StressTestMove*)packet;

		//		tagNCS_StressTestMove msg;
		//		msg.dir			= msg_data_ptr->dir;
		//		msg.src			= msg_data_ptr->src;
		//		msg.flag		= msg_data_ptr->flag;
		//		msg.flag2		= msg_data_ptr->flag2;
		//		msg.time_stamp	= msg_data_ptr->time_stamp;

		//		++g_send_packet;

		//		if (false == g_ncserver.send_msg(session_id, &msg, sizeof(msg)))
		//		{
		//			++g_send_error;
		//			std::cout << "send error!" << std::endl;
		//		}
		//	}
		//	break;
		//default:
		//	{
		//		std::cout << "error msg id" << std::endl;
		//	}
		//	break;
		//}
		//// ÊÍ·Åmsg
		//msg_ptr->pool_ptr->free(msg_ptr);

		if (false == g_nworld.push_msg(msg_ptr))
		{
			g_nperfmon.msg_overflow();
	}
	}

	void ncserver::on_disconnect(uint32 session_id)
	{
		// test close
		// close(session_id);

		tagNCI_Disconnect* msg_ptr = alloc_msg<tagNCI_Disconnect>();

		if (!msg_ptr)
		{
			std::cout << "mem pool alloc error" << std::endl;
			return;
		}
	
		nmessage* _msg_ptr	= NS2MSG(msg_ptr);
		_msg_ptr->session_id	= session_id;

		g_nworld.push_msg(NS2MSG(msg_ptr));
	}

	ncserver g_ncserver;

} // namespace nexus