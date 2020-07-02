#include "nhandler_common.h"

#include "nmsg_loading.h"

namespace nexus {

	void nclient_session::handle_get_init_stat(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_GetInitStat message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_GetInitStat));

		CHECK_MSG_SIZE(msg_ptr, C2S_GetInitStat);

		m_player_ptr->send_init_stat();
	}

	void nclient_session::handle_get_init_spell(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_GetInitSpell message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_GetInitSpell));

		CHECK_MSG_SIZE(msg_ptr, C2S_GetInitSpell);

		m_player_ptr->send_init_spell();
	}

	void nclient_session::handle_get_init_aura(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_GetInitAura message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_GetInitAura));

		CHECK_MSG_SIZE(msg_ptr, C2S_GetInitAura);

		m_player_ptr->send_init_aura();
	}

} // namespace nexus
