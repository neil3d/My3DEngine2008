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
		set_uint32(ECI_WanPort,			cfg_file.get_uint32_default(_T("wan_port"), 6000));
		set_uint32(ECI_MaxSession,		cfg_file.get_uint32_default(_T("max_session"), 4000));
		set_uint32(ECI_MaxLanSession,	cfg_file.get_uint32_default(_T("max_lan_session"), 3));
		set_string(ECI_NWorldIp,		cfg_file.get_string_default(_T("nworld_ip"), "127.0.0.1"));
		set_uint32(ECI_NWorldPort,		cfg_file.get_uint32_default(_T("nworld_port"), 6010));
		set_int32(ECI_Minilzo,			cfg_file.get_uint32_default(_T("minilzo"), 1));

		return true;
	}

	void ncfg::destroy()
	{
		nconfig::destroy();
	}

} // namespace nexus
