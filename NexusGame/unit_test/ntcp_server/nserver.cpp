#include "nserver.h"
#include "nmsg_queue.h"
#include "echo_test.h"
#include "nworld.h"
#include "nsession.h"
#include "nsession_mgr.h"

#include "../ncluster_server/nperfmon.h"

#include <iostream>

namespace nexus {

	nserver::nserver()
	{
	}

	nserver::~nserver()
	{
	}

	void nserver::on_connect(uint32 session_id, uint32 address, uint32 port)
	{
		//if (NULL == g_nsession_mgr.add_session(session_id))
		//{
		//	std::cout << "insert session mgr error!" << std::endl;
		//}

		//// 统计连接数
		//g_nperfmon.session_mod(true);

		//return;


		tagNI_Connect* msg_ptr = alloc_msg<tagNI_Connect>();

		if (!msg_ptr)
		{
			std::cout << "mem pool alloc error" << std::endl;
			return;
		}

		msg_ptr->address	= address;
		msg_ptr->port		= port;

		nmessage* _msg_ptr	= NS2MSG(msg_ptr);
		_msg_ptr->session_id	= session_id;

		// test close & close send
		//close(session_id);
		//tagNS_Chat* rep_ptr = g_nserver.alloc_msg<tagNS_Chat>();

		//if (rep_ptr)
		//{
		//	std::string str = "nexus network test, replay send!";
		//	rep_ptr->len = uint16(str.length());
		//	memcpy(rep_ptr->msg, str.c_str(), str.length());

		//	g_nserver.send(session_id, NS2MSG(rep_ptr), sizeof(tagNS_Chat) - sizeof(uint64));
		//}

		// return;

		g_nworld.push_msg(_msg_ptr);
	}

	void nserver::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// test send
		//msg_ptr->reference = 1;
		//send(session_id, msg_ptr, size);
		//return;

		// test close
		//send(session_id, msg_ptr, size);
		//close(session_id);
		//return;

		//nsession* session_ptr = g_nsession_mgr.get_session(session_id);

		//if (!session_ptr)
		//{
		//	g_nperfmon.msg_error();
		//	return;
		//}

		//if (false == session_ptr->check_message(reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id))))
		//{
		//	g_nperfmon.msg_error();
		//	return;
		//}

		g_nperfmon.recv(size);


		//// 直接回响

		//const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));


		//if (NC_MoveTest != packet->msg_id)
		//{
		//	g_nperfmon.msg_error();
		//}

		//g_nworld._handle_move_test(packet);
		//msg_ptr->free();
		//return;


		if (false == g_nworld.push_msg(msg_ptr))
		{
			g_nperfmon.msg_overflow();
	}
	}

	void nserver::on_disconnect(uint32 session_id)
	{
		//g_nsession_mgr.remove_session(session_id);

		//// 统计连接数
		//g_nperfmon.session_mod(false);

		//return;

		// test close
		// close(session_id);

		tagNI_Disconnect* msg_ptr = alloc_msg<tagNI_Disconnect>();

		if (!msg_ptr)
		{
			std::cout << "mem pool alloc error" << std::endl;
			return;
		}

		nmessage* _msg_ptr	= NS2MSG(msg_ptr);
		_msg_ptr->session_id = session_id;

		g_nworld.push_msg(_msg_ptr);
	}

	nserver g_nserver;

} // namespace nexus