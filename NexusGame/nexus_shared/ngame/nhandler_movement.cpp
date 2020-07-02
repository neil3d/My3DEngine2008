#include "nhandler_common.h"
#include "nunit.h"
#include "nmap.h"

#include "nmsg_movement.h"

namespace nexus {

	void nclient_session::handle_player_move(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_Move message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_Move));

		CHECK_MSG_SIZE(msg_ptr, C2S_Move);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_Move);

		// NLOG_DEBUG(_T("recv C2S_Move message, flag = %u"), uint32(data_ptr->flag));
		// 解析移动消息，调用统一移动操作接口
		m_player_ptr->move(data_ptr->position, vector3(0.0f, data_ptr->yaw, 0.0f), data_ptr->acceleration, data_ptr->time_stamp, data_ptr->flag);
	}

	void nclient_session::handle_check_time(const nmsg_base* msg_ptr)
	{
		CHECK_MSG_SIZE(msg_ptr, C2S_CheckTime);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CheckTime);

		tagS2C_CheckTime reply;
		reply.client_time_stamp = data_ptr->client_time_stamp;
		reply.server_time_stamp = g_game_timer.get_time_stamp();
		send_message(&reply, sizeof(reply));
	}

} // namespace nexus
