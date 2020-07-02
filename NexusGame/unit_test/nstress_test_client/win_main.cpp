#include "nstress_client.h"
#include "echo_test.h"
#include "nclient_session_mgr.h"
#include "nlog_mt.h"
#include "ncfg.h"

#include "../ncluster_server/nperfmon.h"

#include <iostream>

using namespace nexus;

int main(int argc, char* argv[])
{
	if (false == NLOG.init(_T("nstress_test_client"), ELT_Debug, ELT_Debug))
	{
		std::cout << "log init error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	if (false == sncfg.init(_T("test.cfg"), ECI_Max))
	{
		NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
		::Sleep(2000);
		return 1;
	}

	g_nclient_session_mgr.init(1000);

	if (false == g_nstress_client.startup())
	{
		std::cout << "client startup error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	uint32 session_id = 0;

	for (uint32 i = 0; i < sncfg.get_uint32(ECI_MaxSession); ++i)
	{
		session_id = g_nstress_client.connect_to(sncfg.get_string(ECI_ServerIp), 6000);

		if (NULL == g_nclient_session_mgr.add_session(session_id))
		{
			std::cout << "connect error!" << std::endl;
	}
		else
		{
			g_nperfmon.connect();
		}
	}

	uint32 sleep_time = 1000 / sncfg.get_uint32(ECI_Fps);

	uint32 j = 0;

	uint32 real_cur_time	= 0;
	uint32 real_pre_time	= ::GetTickCount();
	uint32 pre_sleep_time	= 0; 

	uint32 loop_counter	= 0; 
	uint32 start_time	= real_pre_time; 

	::Sleep(1000 * 3);

	while (true)
	{
		++loop_counter;
		real_pre_time	= ::GetTickCount();

		g_nclient_session_mgr.send_test();

		//if (++j == 50)
		//{
		//	break;
		//}

		if (real_cur_time - real_pre_time < sleep_time)
		{
			::Sleep(sleep_time - real_cur_time + real_pre_time);
	}

		real_cur_time	= ::GetTickCount();

		g_nperfmon.update(real_cur_time - real_pre_time);
	}

	g_nstress_client.shutdown();
	NLOG.destroy();
	return 0;
}