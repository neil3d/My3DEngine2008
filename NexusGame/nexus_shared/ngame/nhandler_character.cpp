#include "nhandler_common.h"

#include "nmsg_char.h"

namespace nexus {

	void nclient_session::handle_char_enum(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_CharEnum message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CharEnum));

		CHECK_MSG_SIZE(msg_ptr, C2S_CharEnum);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CharEnum);

		tagS2C_CharEnum	reply;
		send_message(&reply, sizeof(reply));
	}

	void nclient_session::handle_char_create(const nmsg_base* msg_ptr)
	{
		NLOG_DEBUG(_T("recv C2S_CharCreate message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CharCreate));

		CHECK_MSG_SIZE(msg_ptr, C2S_CharCreate);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CharCreate);

		tagS2C_CharCreate	reply;
		send_message(&reply, sizeof(reply));
	}

	void nclient_session::handle_char_delete(const nmsg_base* msg_ptr)
	{
		NLOG_DEBUG(_T("recv C2S_CharDelete message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CharDelete));

		CHECK_MSG_SIZE(msg_ptr, C2S_CharDelete);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CharDelete);

		tagS2C_CharDelete	reply;
		send_message(&reply, sizeof(reply));
	}

	void nclient_session::handle_char_enter_game(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_CharEnterGame message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CharEnterGame));

		CHECK_MSG_SIZE(msg_ptr, C2S_CharEnterGame);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CharEnterGame);

		// 校验player id的合法性(角色列表中是否存在)

		// 创建player
		nplayer* player_ptr = new nplayer;

		if (!player_ptr)
		{
			NLOG_ERROR(_T("alloc nplayer memory error, plaeyr_id = %lu"), data_ptr->player_id);
			return;
		}

		if (!player_ptr->init(this, data_ptr->player_id))
		{
			NLOG_ERROR(_T("nplayer init error, plaeyr_id = %lu"), data_ptr->player_id);
			return;
		}

		// 设置session对应的player指针
		m_player_ptr = player_ptr;
		m_player_ptr->request_load_data_from_db();
	}

} // namespace nexus
