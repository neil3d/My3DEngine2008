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
		set_string(ECI_IP,				cfg_file.get_string_default(_T("ip"), "127.0.0.1"));
		set_uint32(ECI_Port,			cfg_file.get_uint32_default(_T("port"), 6000));
		set_uint32(ECI_MaxSession,		cfg_file.get_uint32_default(_T("max_session"), 100));
		set_tstring(ECI_ResPath,		cfg_file.get_tstring_default(_T("res_path"), _T("../../res/")));

		return true;
	}

	void ncfg::destroy()
	{
		nconfig::destroy();
	}

} // namespace nexus
