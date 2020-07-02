#include "nbroadcast_helper.h"
#include "nlan_server.h"
#include "nclient_session.h"
#include "nplayer.h"
#include "nlog_mt.h"

#include "nmsg_world_gateway.h"

namespace nexus {

	nbroadcast_helper::nbroadcast_helper()
	{
		memset(m_session_ids, 0, sizeof(m_session_ids));
		memset(m_msg_ptrs, 0, sizeof(m_msg_ptrs));
	}

	nbroadcast_helper::~nbroadcast_helper()
	{
	}

	bool nbroadcast_helper::send_msg(nplayer* player_ptr, const void* msg_ptr, uint32 size)
	{
		if (!player_ptr)
		{
			return false;
		}

		nclient_session* session_ptr = player_ptr->get_session();

		if (!session_ptr)
		{
			return false;
		}

		uint32 index = session_ptr->get_lan_session_id() & 0X0000FFFF;

		if (index >= MAX_GATEWAY_NUM)
		{
			NLOG_ERROR(_T("client id error, index = %u"), index);
			return false;
		}

		if (m_msg_ptrs[index])
		{
			tagMsg_G2W_Broadcast* packet = reinterpret_cast<tagMsg_G2W_Broadcast*>(&(m_msg_ptrs[index]->session_id));
			packet->push_client(session_ptr->get_wan_session_id());
		}
		else
		{
			m_session_ids[index] = session_ptr->get_lan_session_id();
			m_msg_ptrs[index] = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + sizeof(tagMsg_G2W_Broadcast)));

			if (!m_msg_ptrs[index])
			{
				NLOG_ERROR(_T("alloc nmessage error"));
				return false;
			}

			m_msg_ptrs[index]->pool_ptr		= &m_pool;
			m_msg_ptrs[index]->reference	= 1;				// 引用计数1的设置，在调用send之前
			m_msg_ptrs[index]->size			= sizeof(tagMsg_G2W_Broadcast);
			m_msg_ptrs[index]->session_id	= 0;
			m_msg_ptrs[index]->server_id	= 0;

			// 拷贝数据，注意起始地址, 先拷贝消息再拷贝id
			tagMsg_G2W_Broadcast* packet = reinterpret_cast<tagMsg_G2W_Broadcast*>(&(m_msg_ptrs[index]->session_id));
			packet->push_msg(msg_ptr, size);
			// 修正msg id
			packet->set_original_msg_id(static_cast<const nmsg_base*>(msg_ptr)->msg_id);
			packet->msg_id = Msg_G2W_Broadcast;
			packet->push_client(session_ptr->get_wan_session_id());
		}

		return true;
	}

	void nbroadcast_helper::broadcast()
	{
		for (uint32 i = 0; i < MAX_GATEWAY_NUM; ++i)
		{
			if (m_msg_ptrs[i])
			{
				if (!g_lan_server.send(m_session_ids[i], m_msg_ptrs[i], reinterpret_cast<tagMsg_G2W_Broadcast*>(&(m_msg_ptrs[i]->session_id))->get_packet_size()))
				{
					NLOG_NERROR(_T("%s, send gateway error!"), __TFUNCTION__);
			}
		}
		}

		memset(m_session_ids, 0, sizeof(m_session_ids));
		memset(m_msg_ptrs, 0, sizeof(m_msg_ptrs));
	}

} // namespace nexus
