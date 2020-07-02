#include "nclient_session.h"
#include "nstress_client.h"
#include "echo_test.h"

#include "../ncluster_server/nperfmon.h"

#include <iostream>

namespace nexus {

	nclient_session::nclient_session() : m_session_id(0XFFFFFFFF), m_connected(false), m_packet_id(0), m_recv_packet_id(0)
	{
	}

	nclient_session::~nclient_session()
	{
	}

	bool nclient_session::init(uint32 session_id)
	{
		m_session_id = session_id;
		m_connected = false;
		m_packet_id = 0;
		m_recv_packet_id = 0;
		return true;
	}

	void nclient_session::destroy()
	{
		m_session_id = 0XFFFFFFFF;
		m_connected = false;
		m_packet_id = 0;
		m_recv_packet_id = 0;
	}

	bool nclient_session::send_message(const void* msg_ptr, uint32 size)
	{
		if (!msg_ptr || 0 == size)
		{
			return false;
		}

		return g_nstress_client.send_msg(m_session_id, msg_ptr, size);
	}

	void nclient_session::send_move_test()
	{
		tagNC_MoveTest	req;
		req.src.x = 1.0f;
		req.src.y = 2.0f;
		req.src.z = 3.0f;
		req.dir.x = 1.0f;
		req.dir.y = 2.0f;
		req.dir.z = 3.0f;
		req.flag  = m_packet_id;
		req.flag2 = 2000;
		req.time_stamp	= ::GetTickCount();

		if (false == send_message(&req, sizeof(req)))
		{
			g_nperfmon.send_error();
		}
		else
		{
			g_nperfmon.send(sizeof(req));
		}

		++m_packet_id;
	}

	void nclient_session::send_load_test()
	{
		tagNC_LoadTest	req;
		req.time_stamp	= ::GetTickCount();

		if (false == send_message(&req, sizeof(req)))
		{
			g_nperfmon.send_error();
		}
		else
		{
			g_nperfmon.send(sizeof(req));
		}
	}

	void nclient_session::send_cmd_test()
	{
		tagNC_CmdTest	req;
		req.player_id	= 0;
		req.item_id		= 0;
		req.time_stamp	= ::GetTickCount();

		if (false == send_message(&req, sizeof(req)))
		{
			g_nperfmon.send_error();
		}
		else
		{
			g_nperfmon.send(sizeof(req));
		}
	}

	bool nclient_session::check_recv_packet_id(uint32 packet_id)
	{
		if (m_recv_packet_id != packet_id)
		{
			return false;
		}

		++m_recv_packet_id;
		return true;
	}

} // namespace nexus