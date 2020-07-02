#include "nstress_client.h"
#include "echo_test.h"
#include "nclient_session.h"
#include "nclient_session_mgr.h"

#include "../ncluster_server/nperfmon.h"

#include <iostream>

namespace nexus {

	nstress_client::nstress_client()
	{
	}

	nstress_client::~nstress_client()
	{
	}

	void nstress_client::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		// 回调函数里调用close 主要是测试shutdown是否能够退出
		// close(session_id);

		switch (param1)
		{
		case ECS_Connected:
			{
				nclient_session* session_ptr = g_nclient_session_mgr.get_session(session_id);

				if (!session_ptr)
				{
					g_nperfmon.send_error();
					return;
			}

				session_ptr->set_connected(true);
				g_nperfmon.session_mod(true);


				// std::cout << "it is successful to connect to the server!" << std::endl;
			}
			break;
		case ECS_ConnectTimeout:
			{
				std::cout << "the client is timeout when connecting to the server!" << std::endl;
			}
			break;
		case ECS_ConnectAborted:
			{
				std::cout << "the connect operator is aborted!" << std::endl;
			}
			break;
		case ECS_Error:
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

	void nstress_client::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// 正常情况将数据拷贝到消息队列，目前测试只是打印收到的信息
		
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(msg_ptr->buffer);

		switch (packet->msg_id)
		{
		case NS_Chat:
			{
				if (size != sizeof(tagNS_Chat))
				{
					std::cout << "msg size error!" << std::endl;
				}

				const tagNS_Chat* msg_data_ptr = static_cast<const tagNS_Chat*>(packet);
				char buff[300] = {0};
				memcpy(buff, msg_data_ptr->msg, msg_data_ptr->len);
				std::cout << session_id << ": " << buff << std::endl;
			}
			break;
		case NS_MoveTest:
			{
				if (size != sizeof(tagNS_MoveTest))
				{
					std::cout << "msg size error!" << std::endl;
				}

				const tagNS_MoveTest* msg_data_ptr = static_cast<const tagNS_MoveTest*>(packet);

				if (msg_data_ptr->flag2 != 2000)
				{
					std::cout << session_id << "recv error data!" << std::endl;
				}

				g_nperfmon.recv(size);

				uint32 lag_time = ::GetTickCount() - msg_data_ptr->time_stamp;
				g_nperfmon.lag(lag_time);

				// 校验消息的内容和是否丢失逻辑包

				//nclient_session* session_ptr = g_nclient_session_mgr.get_session(session_id);

				//if (!session_ptr)
				//{
				//	g_nperfmon.msg_error();
				//	return;
				//}

				//if (false == session_ptr->check_recv_packet_id(msg_data_ptr->flag))
				//{
				//	g_nperfmon.msg_error();
				//	return;
				//}
				}
			break;
		case NS_LoadTest:
			{
				if (size != sizeof(tagNS_LoadTest))
				{
					std::cout << "msg size error!" << std::endl;
				}

				const tagNS_LoadTest* msg_data_ptr = static_cast<const tagNS_LoadTest*>(packet);

				g_nperfmon.recv(size);

				uint32 lag_time = ::GetTickCount() - msg_data_ptr->time_stamp;
				g_nperfmon.lag(lag_time);
			}
			break;
		case NS_CmdTest:
			{
				if (size != sizeof(tagNS_CmdTest))
				{
					std::cout << "msg size error!" << std::endl;
				}

				const tagNS_CmdTest* msg_data_ptr = static_cast<const tagNS_CmdTest*>(packet);

				g_nperfmon.recv(size);

				uint32 lag_time = ::GetTickCount() - msg_data_ptr->time_stamp;
				g_nperfmon.lag(lag_time);
			}
			break;
		default:
			{
				g_nperfmon.msg_error();
			}
			break;
		}

		msg_ptr->free();
		// on_datarecv close
		// close(session_id);
	}

	void nstress_client::on_disconnect(uint32 session_id)
	{
		// std::cout << "disconnect" << std::endl;
		g_nclient_session_mgr.remove_session(session_id);
		g_nperfmon.session_mod(false);
	}

	bool nstress_client::send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
	{
		nmessage* nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + size));

		if (NULL == nmsg_ptr)
		{
			return false;
		}

		nmsg_ptr->pool_ptr		= &m_pool;
		nmsg_ptr->next_ptr		= NULL;
		nmsg_ptr->reference		= 1;				// 引用计数1的设置，在调用send之前
		nmsg_ptr->size			= size;
		nmsg_ptr->session_id	= 0;
		nmsg_ptr->server_id		= 0;

		// 拷贝数据，注意起始地址
		memcpy(nmsg_ptr->buffer, msg_ptr, size);

		return send(session_id, nmsg_ptr, size);
	}

	nstress_client g_nstress_client;

} // namespace nexus
