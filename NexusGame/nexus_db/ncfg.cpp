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
		set_string(ECI_WorldIP,					cfg_file.get_string_default(_T("nworld_ip"), "127.0.0.1"));
		set_uint32(ECI_WorldPort,				cfg_file.get_uint32_default(_T("nworld_port"), 6010));
		set_uint32(ECI_MaxLanSession,			cfg_file.get_uint32_default(_T("max_lan_session"), 1));

		set_string(ECI_DatabaseHost,			cfg_file.get_string_default(_T("database_host"), "172.16.11.254"));
		set_string(ECI_DatabasePort,			cfg_file.get_string_default(_T("database_port"), "3306"));
		set_string(ECI_DatabaseName,			cfg_file.get_string_default(_T("database_name"), "nexus_game"));
		return true;
	}

	void ncfg::destroy()
	{
		nconfig::destroy();
	}

} // namespace nexus
