#include "ncclient.h"
#include "cluster_echo_test.h"
#include "../ncluster_server/nperfmon.h"

#include <iostream>

namespace nexus {

	ncclient::ncclient()
	{
	}

	ncclient::~ncclient()
	{
	}

	void ncclient::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		// test close
		//close(session_id);
		//return;

		//static bool tag = false;

		//if (false == tag)
		//{
		//	g_ncclient.connect_to("127.0.0.1", 6011);
		//	tag = true;
		//}

		//tagNCC_Chat test_send_msg;
		//test_send_msg.len = sizeof("hello");
		//memcpy(test_send_msg.msg, "hello", sizeof("hello"));

		//send_msg(session_id, &test_send_msg, sizeof(test_send_msg));

		switch (param1)
		{
		case ECC_Connected:
			{
				std::cout << "it is successful to connect to the server!" << std::endl;
			}
			break;
		case ECC_ConnectTimeout:
			{
				std::cout << "the client is timeout when connecting to the server!" << std::endl;
			}
			break;
		case ECC_ConnectAborted:
			{
				std::cout << "the client is disconnected!" << std::endl;
			}
			break;
		case ECC_Error:
			{
				std::cout << "the error occur!" << std::endl;
			}
			break;
		default:
			{
				std::cout << "unknown error occur!" << std::endl;
			}
			break;
		}
	}

	void ncclient::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		nmsg_base* packet = reinterpret_cast<nmsg_base*>(&(msg_ptr->session_id));

		// test send
		//tagNCC_Chat test_send_msg;
		//memcpy(test_send_msg.msg, "hello", sizeof("hello"));

		//send_msg(session_id, &test_send_msg, sizeof(test_send_msg));
		//return;

		switch (packet->msg_id)
		{
		case NCS_Chat:
			{
				if (size != sizeof(tagNCS_Chat))
				{
					std::cout << "msg size error!" << std::endl;
				}

				tagNCS_Chat* msg_data_ptr = static_cast<tagNCS_Chat*>(packet);
				char buff[300] = {0};
				memcpy(buff, msg_data_ptr->msg, msg_data_ptr->len);
				std::cout << "recv server msg:" << buff << std::endl;
			}
			break;
		case NCS_Reply:
			{
				if (size != sizeof(tagNCS_Reply))
				{
					std::cout << "msg size error!" << std::endl;
				}

				tagNCS_Reply* msg_data_ptr = static_cast<tagNCS_Reply*>(packet);

				// 数据校验
				static uint32 i = 0;

				if (i != msg_data_ptr->type_ui32)
				{
					g_nperfmon.msg_error();
				}

				++i;

				g_nperfmon.recv(size);
				// break;

				//tagNCS_Reply* msg_data_ptr = static_cast<tagNCS_Reply*>(packet);
				//std::cout << session_id << ", time:" << ::GetTickCount() - msg_data_ptr->time_stamp << std::endl;
				//std::cout << msg_data_ptr->type_i64 << std::endl;
				//std::cout << msg_data_ptr->type_i32 << std::endl;
				//std::cout << msg_data_ptr->type_i16 << std::endl;
				//// std::cout << msg_data_ptr->type_i8 << std::endl;
				//std::cout << msg_data_ptr->type_ui64 << std::endl;
				//std::cout << msg_data_ptr->type_ui32 << std::endl;
				//std::cout << msg_data_ptr->type_ui16 << std::endl;
				//// std::cout << msg_data_ptr->type_ui8 << std::endl;
			}
			break;
		case NCS_StressTestMove:
			{
				//if (size != sizeof(tagNCS_StressTestMove))
				//{
				//	std::cout << "msg size error!" << std::endl;
				//}

				tagNCS_StressTestMove* msg_data_ptr = static_cast<tagNCS_StressTestMove*>(packet);

				if (size != sizeof(tagNCS_StressTestMove) + msg_data_ptr->session_num * 4)
				{
					std::cout << "msg size error!" << std::endl;
				}

				g_nperfmon.recv(size);

				uint32 lag_time = ::GetTickCount() - msg_data_ptr->time_stamp;
				g_nperfmon.lag(lag_time);
			}
			break;
		default:
			{
				std::cout << "error msg id" << std::endl;
			}
			break;
		}

		msg_ptr->pool_ptr->free(msg_ptr);

		//static bool tag = false;

		//if (false == tag)
		//{
		//	g_ncclient.connect_to("127.0.0.1", 6011);
		//	tag = true;
		//}

		//close(session_id);
		return;
	}

	void ncclient::on_disconnect(uint32 session_id)
	{
		std::cout << "disconnect client_id, " << session_id << std::endl;

		// close(session_id);

		//static bool tag = false;

		//if (false == tag)
		//{
		//	g_ncclient.connect_to("127.0.0.1", 6011);
		//	tag = true;
		//}


		tagNCC_Chat test_send_msg;
		test_send_msg.len = sizeof("hello");
		memcpy(test_send_msg.msg, "hello", sizeof("hello"));

		send_msg(session_id, &test_send_msg, sizeof(test_send_msg));
	}

	ncclient g_ncclient;

} // namespace nexus
