#include "ncfg.h"
#include "nlog_mt.h"

namespace nexus {

	ncfg::ncfg()
	{

	}

	ncfg::~ncfg()
	{
	}

	bool ncfg::init(const tstring& file_name, uint16 values_count)
	{
		if (false == _set_values_count(values_count))
		{
			NLOG_ERROR(_T("init config, set values count error, values_count = %u"), values_count);
			return false;
		}

		nconfig_file cfg_file;
		cfg_file.open(file_name);

			// ≈–∂œ≈‰÷√≤Œ ˝
		set_uint32(ECI_WorldPort,				cfg_file.get_uint32_default(_T("world_port"), 6010));
		set_uint32(ECI_MaxLanSessionClient,		cfg_file.get_uint32_default(_T("max_lan_session_client"), 2));
		set_uint32(ECI_MaxLanSessionServer,		cfg_file.get_uint32_default(_T("max_lan_session_server"), 2));
		set_string(ECI_NLoginIp,				cfg_file.get_string_default(_T("nlogin_ip"), "127.0.0.1"));
		set_uint32(ECI_NLoginPort,				cfg_file.get_uint32_default(_T("nlogin_port"), 6011));
		set_string(ECI_NBgIp,					cfg_file.get_string_default(_T("nbg_ip"), "127.0.0.1"));
		set_uint32(ECI_NBgPort,					cfg_file.get_uint32_default(_T("nbg_port"), 6012));

		set_uint32(ECI_AeraId,					cfg_file.get_uint32_default(_T("aera_id"), 1));
		set_uint32(ECI_BattleGroundId,			cfg_file.get_uint32_default(_T("battle_ground_id"), 1));
		set_uint32(ECI_WorldId,					cfg_file.get_uint32_default(_T("world_id"), 0));
		set_tstring(ECI_WorldName,				cfg_file.get_tstring_default(_T("world_name"), _T("nexus_001")));
		set_tstring(ECI_ResPath,				cfg_file.get_tstring_default(_T("res_path"), _T("../../../res/")));
		set_float(ECI_FloatTest,				cfg_file.get_float_default(_T("float_test"), 10.2f));

			// º∆À„server id
			set_uint32(ECI_ServerId, _calculate_server_id());

		return true;
	}

	void ncfg::destroy()
	{
		nconfig::destroy();
	}

	uint32 ncfg::_calculate_server_id()
	{
		uint32 server_id = 0;
		uint32 value = get_uint32(ECI_AeraId);
		server_id |= (value << 16);
		value = get_uint32(ECI_BattleGroundId);
		server_id |= (value << 8);
		value = get_uint32(ECI_WorldId);
		server_id |= value;
		return server_id;
	}

} // namespace nexus
