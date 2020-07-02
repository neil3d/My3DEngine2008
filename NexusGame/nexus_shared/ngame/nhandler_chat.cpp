#include "nhandler_common.h"
#include "ngm_commands.h"
#include "nmap.h"

#include "nmsg_chat.h"

namespace nexus {

	void nclient_session::handle_player_chat(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_ChatMessage);
		uint32 size = reinterpret_cast<const nmessage*>(reinterpret_cast<const char*>(msg_ptr) - NSMSG_OFFSET)->size;

		// 注意变长消息, 先检查到长度信息的长度, 再检查整体长度
		if (size < sizeof(nmsg_base) + sizeof(uint8) + sizeof(uint8) || size != data_ptr->get_packet_size())
		{
			NLOG_DEBUG(_T("msg size error!"));
			return;
		}

		switch (data_ptr->type)
		{
		case ECT_Channel:
			{
				tagS2C_ChatMessage reply;
				reply.client_id	= m_client_id;
				reply.player_id	= m_player_ptr->get_id();
				reply.type		= ECT_Channel;
				reply.flag		= 0;
				reply.msg2.channel_id	= data_ptr->msg3.channel_id;
				reply.msg_size			= uint8(data_ptr->msg_size > (MAX_MSG_LEN * sizeof(nchar)) ? (MAX_MSG_LEN * sizeof(nchar)) : data_ptr->msg_size);
				memcpy(reply.msg2.msg, data_ptr->msg3.msg, reply.msg_size);

				tagTimeStamp* data2 = (tagTimeStamp*)reply.msg2.msg;
				data2->time_stamp3 = ::timeGetTime();

				send_message(&reply, reply.get_packet_size());
			}
			break;
		case ECT_Whisper:
			{
				// TODO: D. Zhao
			}
			break;
		case ECT_Map:
			{
				nmap* map_ptr = m_player_ptr->get_map();

				if (!map_ptr)
				{
					NLOG_WARNING(_T("player has not enter map!"));
					return;
			}

				tagS2C_ChatMessage reply;
				reply.client_id	= m_client_id;
				reply.player_id	= m_player_ptr->get_id();
				reply.type		= ECT_Map;
				reply.flag		= 0;
				reply.msg_size	= uint8(data_ptr->msg_size > (MAX_MSG_LEN * sizeof(nchar)) ? (MAX_MSG_LEN * sizeof(nchar)) : data_ptr->msg_size);
				memcpy(reply.msg1.msg, data_ptr->msg1.msg, reply.msg_size);

				if (s_gm_command.parse_commands(reply.msg1.msg, this))
				{
					return;
				}

				map_ptr->map_broadcast(m_player_ptr, &reply, reply.get_packet_size(), true);
			}
			break;
		default:
			{

			}
			break;
		}



	}

} // namespace nexus
