#include "ncclient.h"
#include "cluster_echo_test.h"
#include "../ncluster_server/nperfmon.h"
#include "nlog_mt.h"

#include <iostream>

using namespace nexus;

const uint32 SLEEP_TIME = 100;

int main(int argc, char* argv[])
{
	if (false == NLOG.init(_T("ncluster_client_test"), ELT_Debug, ELT_Debug))
	{
		std::cout << "log init error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	g_ncclient.startup();
	uint32 client_id = g_ncclient.connect_to("172.16.11.11", 6010);
	// uint32 client_id2 = g_ncclient.connect_to("172.16.11.12", 6011);


	// 可能还处理acceptex状态投递close会失败
	// g_ncclient.close(client_id);

	tagNCC_Chat req;
	tagNCC_Request	msg_test;

	std::string msg[5] = {"hello", "nexus", "MMORPG", "network", "test"};
	int i = 0, j = 0;

	//while (true)
	//{
	//	if (i >= 5)
	//	{
	//		i = 0;
	//	}

	//	//req.len = uint16(msg[i].length());
	//	//memcpy(req.msg, msg[i].c_str(), msg[i].length());

	//	msg_test.time_stamp = ::GetTickCount();
	//	++msg_test.type_i64;
	//	++msg_test.type_i32;
	//	++msg_test.type_i16;
	//	++msg_test.type_i8;
	//	++msg_test.type_ui64;
	//	++msg_test.type_ui32;
	//	++msg_test.type_ui16; 
	//	++msg_test.type_ui8;

	//	g_ncclient.send_msg(client_id, &msg_test, sizeof(msg_test));
	//	// g_ncclient.send_msg(client_id2, &msg_test, sizeof(msg_test));

	//	//if (++j == 20)
	//	//{
	//	//	break;
	//	//}

	//	++i;
	//	::Sleep(1000);
	//	// g_ncclient.close(client_id);
	//}

	tagNCC_StressTestMove msg_move;

	uint32 real_cur_time	= 0;
	uint32 real_pre_time	= ::GetTickCount();
	uint32 pre_sleep_time	= 0; 

	uint32 loop_counter	= 0; 
	uint32 start_time	= real_pre_time; 
	uint32 send_error = 0;

	::Sleep(1000 * 3);

	while (true)
	{
		++loop_counter;
		real_pre_time	= ::GetTickCount();

		msg_move.src.x = 1.0f;
		msg_move.src.y = 1.0f;
		msg_move.src.z = 1.0f;
		msg_move.dir.x = 1.0f;
		msg_move.dir.y = 1.0f;
		msg_move.dir.z = 1.0f;
		msg_move.flag  = 0;
		msg_move.flag2 = 0;

		msg_test.time_stamp = ::GetTickCount();
		++msg_test.type_i64;
		++msg_test.type_i32;
		++msg_test.type_i16;
		++msg_test.type_i8;
		++msg_test.type_ui64;

		++msg_test.type_ui16; 
		++msg_test.type_ui8;


		for (i = 0; i < 1000; ++i)
		{
			msg_move.time_stamp = ::GetTickCount();
			static uint32 j = 0;
			msg_test.type_ui32 = j;
			++j;

			if (false == g_ncclient.send_msg(client_id, &msg_move, sizeof(msg_move)))
			// if (false == g_ncclient.send_msg(client_id, &msg_test, sizeof(msg_test)))
			{
				g_nperfmon.send_error();
			}
			else
			{
				g_nperfmon.send(sizeof(msg_move));
			}

			//if (false == g_ncclient.send_msg(client_id2, &msg_move, sizeof(msg_move)))
		//{
			//	g_nperfmon.error();
		//}
			//else
			//{
			//	g_nperfmon.send(sizeof(msg_move));
			//}
		}

		if (real_cur_time - real_pre_time < SLEEP_TIME)
		{
			::Sleep(SLEEP_TIME - real_cur_time + real_pre_time);
	}

		real_cur_time	= ::GetTickCount();

		g_nperfmon.update(real_cur_time - real_pre_time);
	}

	::Sleep(1000);

	g_ncclient.shutdown();
	NLOG.destroy();
	return 0;
}