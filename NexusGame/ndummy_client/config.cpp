#include "StdAfx.h"
#include "config.h"

#include "util/string_util.h"

#include <fstream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;


config::config()
{

}

config::~config()
{
}

config* config::instance()
{
	static config s_inst;
	return &s_inst;
}

bool config::init(const tstring& file_name, uint16 values_count)
{
	if (false == _set_values_count(values_count))
	{
		nLog_Info(_T("init config, set values count error, values_count = %u"), values_count);
		return false;
	}

	try 
	{
		po::options_description config;
		config.add_options()
			("server_ip", po::value<std::string>()->default_value("127.0.0.1"), "")				// 
			("server_port", po::value<uint32>()->default_value(6000), "")						// 
			("res_path", po::value<std::string>()->default_value("../res/"), "")						// 
			;

		po::options_description config_file_options;
		config_file_options.add(config);

		po::variables_map vm;

		tifstream ifs(file_name.c_str());

		store(parse_config_file(ifs, config_file_options), vm);
		notify(vm);

		// ≈–∂œ≈‰÷√≤Œ ˝
		set_string(ECI_ServerIP, vm["server_ip"].as<std::string>());
		set_uint32(ECI_ServerPort, vm["server_port"].as<uint32>());

		std::wstring str = conv_string(vm["res_path"].as<std::string>());
		set_tstring(ECI_ResPath, str);
	}
	catch(std::exception& e)
	{
		nLog_Info(_T("init config exception, %u"), e.what());
		return false;
	}

	return true;
}

void config::destroy()
{
	nconfig::destroy();
}
