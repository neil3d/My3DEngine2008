#include "ngm_commands.h"
#include "nclient_session.h"
#include "nlog_mt.h"

namespace nexus {

	void ngm_commands::_handle_reload_config_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 info_id = 0, num = 0;
		float dot = 5.0f;
		tstring str;
		param.get_param(0, info_id);
		param.get_param(1, num);
		param.get_param(2, dot);
		param.get_param(3, str);
		return;
	}


	void ngm_commands::_handle_reload_all_loot_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 info_id = 0, num = 0;
		float dot = 5.0f;
		tstring str;
		param.get_param(0, info_id);
		param.get_param(1, num);
		param.get_param(2, dot);
		param.get_param(3, str);
		return;
	}

} // namespace nexus
