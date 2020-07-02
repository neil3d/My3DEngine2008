#include "nhandler_common.h"
#include "nunit.h"
#include "nspell_aura.h"

#include "nmsg_spell.h"

namespace nexus {

	void nclient_session::handle_cast_spell(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_CastSpell message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CastSpell));

		CHECK_MSG_SIZE(msg_ptr, C2S_CastSpell);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CastSpell);

		m_player_ptr->cast_generic_spell(data_ptr->spell_id, data_ptr->target_id, data_ptr->target_pos, data_ptr->time_stamp, data_ptr->cast_count);
	}

	void nclient_session::handle_cancel_cast(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_CancelCast message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CancelCast));

		CHECK_MSG_SIZE(msg_ptr, C2S_CancelCast);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CancelCast);

		m_player_ptr->cancel_spell();
	}

	void nclient_session::handle_cancel_aura(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_CancelAura message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_CancelAura));

		CHECK_MSG_SIZE(msg_ptr, C2S_CancelAura);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_CancelAura);

		naura* aura_ptr =  NULL;
		nunit::naura_map auras = m_player_ptr->get_auras();

		for (nunit::naura_map::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
		{
			if (data_ptr->slot == iter->second->get_slot())
			{
				aura_ptr = iter->second;
				break;
	}
		}

		// 判断是否存在
		if (!aura_ptr)
		{
			NLOG_DEBUG(_T("get slot aura error, slot = %u"), data_ptr->slot);
			return;
		}

		// 判断是否可手动取消
		if (0 == (aura_ptr->get_interrupt_flags() & EAIF_Cancel))
		{
			NLOG_DEBUG(_T("cant cancel this aura, slot = %u"), data_ptr->slot);
			return;
		}

		// remove aura
		m_player_ptr->remove_aura(aura_ptr);
	}

} // namespace nexus
