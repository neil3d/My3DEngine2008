#include "ncfg.h"
#include "nlog_mt.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

		try 
		{
			po::options_description config;
			config.add_options()
				("server_ip", po::value<std::string>()->default_value("127.0.0.1"), "")		//
				("max_session", po::value<uint32>()->default_value(500), "")	// ECI_MaxSession
				("msg_type", po::value<uint32>()->default_value(0), "")			// 
				("fps", po::value<uint32>()->default_value(7), "")				//
				;

			po::options_description config_file_options;
			config_file_options.add(config);

			po::variables_map vm;

			tifstream ifs(file_name.c_str());
			store(parse_config_file(ifs, config_file_options), vm);
			notify(vm);
		
			// ≈–∂œ≈‰÷√≤Œ ˝
			set_string(ECI_ServerIp, vm["server_ip"].as<std::string>());
			set_uint32(ECI_MaxSession, vm["max_session"].as<uint32>());
			set_uint32(ECI_MsgType, vm["msg_type"].as<uint32>());
			set_uint32(ECI_Fps, vm["fps"].as<uint32>());
		}
		catch(std::exception& e)
		{
			NLOG_ERROR(_T("init config exception, %u"), e.what());
			return false;
		}

		return true;
	}

	void ncfg::destroy()
	{
		nconfig::destroy();
	}

} // namespace nexus
