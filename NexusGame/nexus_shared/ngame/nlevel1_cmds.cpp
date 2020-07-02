#include "ngm_commands.h"
#include "nclient_session.h"
#include "nworld_session.h"
#include "nplayer.h"
#include "nmap_mgr.h"
#include "nmap.h"
#include "nlog_mt.h"

namespace nexus {

	void ngm_commands::_handle_server_info_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		// NLOG_DEBUG(_T("recv serverinfo command"));

		uint32 session_num = g_world_session.get_session_num();

		NLOG_STRING(_T("world info:"));
		NLOG_STRING(_T("\ttotal session num = %u"), session_num);

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
		return;
	}

		nmap* map_ptr = player_ptr->get_map();

		if (!map_ptr)
		{
			return;
		}

		NLOG_STRING(_T("\tmap player num = %u"), map_ptr->get_player_num());
		return;
	}

} // namespace nexus
