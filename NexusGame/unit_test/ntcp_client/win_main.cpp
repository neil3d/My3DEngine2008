#include "nclient.h"
#include "echo_test.h"

#include <iostream>

using namespace nexus;

#include "echo_test.h"
#include "../ncluster_server/nperfmon.h"

const uint32 SLEEP_TIME = 150;

int main(int argc, char* argv[])
{
	g_nclient.startup();

	g_nclient.connect_to("172.16.11.11", 6000);

	// 多次调用测试，里面有线程安全
	// g_nclient.connect_to("127.0.0.1", 6000);

	// test close
	// g_nclient.close();

	tagNC_Chat req;

// 	std::string msg[5] = {"hello", "nexus", "MMORPG", "network", "test"};
	int i = 0;

	//uint32 ptr = 0;
	//*reinterpret_cast<uint32*>(ptr);
	int32 j = 0;

	uint32 real_cur_time	= 0;
	uint32 real_pre_time	= ::GetTickCount();
	uint32 pre_sleep_time	= 0; 

	uint32 loop_counter	= 0; 
	uint32 start_time	= real_pre_time; 

	::Sleep(1000 * 3);


	while (true)
	{
		//req.len = uint16(msg[i].length());
		//memcpy(req.msg, msg[i].c_str(), msg[i].length());

		++loop_counter;
		real_pre_time	= ::GetTickCount();

		if (g_connected)
		{
			for (j = 0; j < 1; ++j)
			{
				tagNC_MoveTest	req;
				req.src.x = 1.0f;
				req.src.y = 2.0f;
				req.src.z = 3.0f;
				req.dir.x = 1.0f;
				req.dir.y = 2.0f;
				req.dir.z = 3.0f;
				req.flag  = i;
				req.flag2 = 2000;
				req.time_stamp	= ::GetTickCount();
				// req.time_stamp	= ::timeGetTime();
				++i;

				if (false == g_nclient.send(&req, sizeof(req)))
				{
					g_nperfmon.send_error();
		}
				else
				{
					g_nperfmon.send(sizeof(req));
				}
			}
		}

		if (real_cur_time - real_pre_time < SLEEP_TIME)
		{
			::Sleep(SLEEP_TIME - real_cur_time + real_pre_time);
		}

		real_cur_time	= ::GetTickCount();

		g_nperfmon.update(real_cur_time - real_pre_time);


		//if (g_connected)
		//{
		//	for (j = 0; j < 1; ++j)
		//	{
		//		tagNC_MoveTest	req;
		//		req.src.x = 1.0f;
		//		req.src.y = 2.0f;
		//		req.src.z = 3.0f;
		//		req.dir.x = 1.0f;
		//		req.dir.y = 2.0f;
		//		req.dir.z = 3.0f;
		//		req.flag  = i;
		//		req.flag2 = 2000;
		//		req.time_stamp	= ::GetTickCount();
		//		++i;
		//		if (false == g_nclient.send(&req, sizeof(req)))
		//		{
		//			std::cout << "send error!" << std::endl;
		//}
		//	}
		//}


		////if (++j == 50)
		////{
		////	break;
		////}

		//::Sleep(20);
	}

	g_nclient.close();

	g_nclient.shutdown();
	return 0;
}