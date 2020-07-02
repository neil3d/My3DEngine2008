#include "nworld.h"
#include "echo_test.h"
#include "nserver.h"
#include "nsession_mgr.h"
#include "nsession.h"

#include "../ncluster_server/nperfmon.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <iostream>

namespace nexus {

	nworld::nworld() : m_terminate_update(false), m_msg_queue(30000), m_cur_head_ptr(NULL), m_max_tick_msg(0)
	{
	}

	nworld::~nworld()
	{
	}

	bool nworld::init(uint32 max_tick_msg)
	{
		m_terminate_update = false;
		m_max_tick_msg = max_tick_msg;

		m_threads.create_thread(boost::bind(&nworld::_thread_update, this));
		return true;
	}

	void nworld::destroy()
	{
		m_terminate_update = true;

		m_threads.join_all();
	}

	void nworld::_thread_update()
	{
		::timeBeginPeriod(1);
		uint32 real_cur_time	= 0;
		uint32 real_pre_time	= ::timeGetTime();
		uint32 pre_sleep_time	= 0; 

		while (!m_terminate_update)
		{
			real_cur_time = ::timeGetTime();

			uint32 diff = real_cur_time - real_pre_time;

			// std::cout << "updata diff time = " << diff << std::endl;

			_handle_msg();
			g_nperfmon.update(diff);

			real_pre_time = real_cur_time;

			if (diff <= 50 + pre_sleep_time)
			{
				pre_sleep_time = 50 + pre_sleep_time - diff;
				::Sleep(pre_sleep_time);

				// std::cout << "sleep time = " << pre_sleep_time << std::endl;
		}
			else
			{
				pre_sleep_time = 0;
	}
		}

		::timeEndPeriod(1);
	}

	void nworld::_handle_msg()
	{
		uint32 time_begin = ::GetTickCount();
		uint32 msg_count = 0;

		if (!m_cur_head_ptr)
		{
			m_cur_head_ptr = m_msg_queue.dequeue();
		}

		nmessage* cur_msg_ptr = m_cur_head_ptr;
		nmessage* msg_ptr = NULL;

		while (cur_msg_ptr)
		{
			if (msg_count >= m_max_tick_msg)
			{
				break;
			}

			++msg_count;

			msg_ptr = cur_msg_ptr;
			// ------------------ begin -----------------//

			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(cur_msg_ptr->session_id));

			switch (packet->msg_id)
			{
			case NI_Connect:
				{
					//if (false == m_tag)
					//{
					//	m_session_id = cur_msg_ptr->session_id;
					//	m_tag = true;
					//}

					// std::cout << "connect create!" << std::endl;

					_handle_create_session(packet);
				}
				break;
			case NI_Disconnect:
				{
					// std::cout << "connect close!" << std::endl;

					_handle_destory_session(packet);
				}
				break;
			case NC_Chat:
				{
					_handle_chat_test(packet);
				}
				break;
			case NC_MoveTest:
				{
					_handle_move_test(packet);
				}
				break;
			case NC_LoadTest:
				{
					_handle_load_test(packet);
				}
				break;
			case NC_CmdTest:
				{
					_handle_cmd_test(packet);
				}
				break;
			default:
				{
					std::cout << "error msg id" << std::endl;
				}
				break;
			}


			// ------------------ end -----------------//

			cur_msg_ptr = cur_msg_ptr->next_ptr;
			// 释放msg
			msg_ptr->pool_ptr->free(msg_ptr);
		}

		m_cur_head_ptr = cur_msg_ptr;
		g_nperfmon.msg_proc(msg_count, ::GetTickCount() - time_begin);
	}


	void nworld::_handle_create_session(const nmsg_base* packet)
	{
		const tagNI_Connect* data_ptr = static_cast<const tagNI_Connect*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);

		if (NULL == g_nsession_mgr.add_session(session_id))
		{
			std::cout << "insert session mgr error!" << std::endl;
		}

		// 统计连接数
		g_nperfmon.session_mod(true);
	}

	void nworld::_handle_destory_session(const nmsg_base* packet)
	{
		const tagNI_Disconnect* data_ptr = static_cast<const tagNI_Disconnect*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);
		
		g_nsession_mgr.remove_session(session_id);

		// 统计连接数
		g_nperfmon.session_mod(false);
	}

	void nworld::_handle_chat_test(const nmsg_base* packet)
	{
		const tagNC_Chat* data_ptr = static_cast<const tagNC_Chat*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);

					char buff[256] = {0};
		memcpy(buff, data_ptr->msg, data_ptr->len);
					std::string str(buff);

		std::cout << data_ptr->client_id << ": " << str << std::endl;

					tagNS_Chat* rep_ptr = g_nserver.alloc_msg<tagNS_Chat>();

					if (rep_ptr)
					{
						str += ", nexus network test, replay send!";
						rep_ptr->len = uint16(str.length());
						memcpy(rep_ptr->msg, str.c_str(), str.length());

						// g_nserver.send(m_session_id, NS2MSG(rep_ptr), sizeof(tagNS_Chat) - sizeof(uint64));
			g_nserver.send(session_id, NS2MSG(rep_ptr), sizeof(tagNS_Chat) - sizeof(uint64));
					}
				}

	void nworld::_handle_move_test(const nmsg_base* packet)
				{
		const tagNC_MoveTest* data_ptr = static_cast<const tagNC_MoveTest*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);

		assert(data_ptr->flag2 == 2000);

		nsession* session_ptr = g_nsession_mgr.get_session(session_id);

		if (!session_ptr)
		{
			g_nperfmon.msg_error();
			return;
		}

		if (false == session_ptr->check_message(packet))
		{
			g_nperfmon.msg_error();
			return;
		}

		uint32 session_ids[100];
		uint32 session_num = 0, i = 0;

		g_nsession_mgr.get_broadcast_seesion_id(session_ids, session_num);
		nmessage* nmsg_ptr = NULL;

		tagNS_MoveTest* rep_ptr = g_nserver.alloc_msg<tagNS_MoveTest>();

		if (rep_ptr)
		{
			rep_ptr->player_id = 0;
			rep_ptr->src.x = data_ptr->src.x;
			rep_ptr->src.y = data_ptr->src.y;
			rep_ptr->src.z = data_ptr->src.z;
			rep_ptr->dir.x = data_ptr->dir.x;
			rep_ptr->dir.y = data_ptr->dir.y;
			rep_ptr->dir.z = data_ptr->dir.z;
			rep_ptr->flag  = data_ptr->flag;
			rep_ptr->flag2 = data_ptr->flag2;
			rep_ptr->time_stamp = data_ptr->time_stamp;

			nmsg_ptr = NS2MSG(rep_ptr);

			nmsg_ptr->reference = session_num;

			for (i = 0; i < session_num; ++i)
			{
				g_nperfmon.send(sizeof(tagNS_MoveTest) - sizeof(uint64));

				if (false == g_nserver.send(session_id, nmsg_ptr, sizeof(tagNS_MoveTest) - sizeof(uint64)))
				{
					g_nperfmon.send_error();
		}
	}
	}
	}

	void nworld::_handle_load_test(const nmsg_base* packet)
	{
		const tagNC_LoadTest* data_ptr = static_cast<const tagNC_LoadTest*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);

		nsession* session_ptr = g_nsession_mgr.get_session(session_id);

		if (!session_ptr)
		{
			g_nperfmon.msg_error();
			return;
		}

		tagNS_LoadTest* rep_ptr = g_nserver.alloc_msg<tagNS_LoadTest>();

		if (rep_ptr)
		{
			rep_ptr->time_stamp = data_ptr->time_stamp;

			g_nperfmon.send(sizeof(tagNS_LoadTest) - sizeof(uint64));

			if (false == g_nserver.send(session_id, NS2MSG(rep_ptr), sizeof(tagNS_LoadTest) - sizeof(uint64)))
			{
				g_nperfmon.send_error();
			}
		}
	}

	void nworld::_handle_cmd_test(const nmsg_base* packet)
	{
		const tagNC_CmdTest* data_ptr = static_cast<const tagNC_CmdTest*>(packet);
		uint32 session_id = static_cast<uint32>(data_ptr->client_id & 0X00000000FFFFFFFFui64);

		nsession* session_ptr = g_nsession_mgr.get_session(session_id);

		if (!session_ptr)
		{
			g_nperfmon.msg_error();
			return;
		}

		tagNS_CmdTest* rep_ptr = g_nserver.alloc_msg<tagNS_CmdTest>();

		if (rep_ptr)
		{
			rep_ptr->time_stamp = data_ptr->time_stamp;

			g_nperfmon.send(sizeof(tagNS_CmdTest) - sizeof(uint64));

			if (false == g_nserver.send(session_id, NS2MSG(rep_ptr), sizeof(tagNS_CmdTest) - sizeof(uint64)))
			{
				g_nperfmon.send_error();
			}
		}
	}

	nworld g_nworld;

} // namespace nexus