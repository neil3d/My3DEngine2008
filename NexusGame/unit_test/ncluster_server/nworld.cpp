#include "nworld.h"
#include "cluster_echo_test.h"
#include "ncserver.h"
#include "nperfmon.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <iostream>

namespace nexus {


	nworld::nworld() : m_terminate_update(false), m_msg_queue(30000), m_cur_head_ptr(NULL), m_proc_msg_tick(0), m_tag(false), m_session_id(0) 
	{

	}

	bool nworld::init()
	{
		m_terminate_update = false;

		m_proc_msg_tick = 10000;

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
		uint32 real_cur_time	= 0;
		uint32 real_pre_time	= ::GetTickCount();
		uint32 pre_sleep_time	= 0; 

		while (!m_terminate_update)
		{
			real_cur_time = ::GetTickCount();

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
			if (msg_count >= m_proc_msg_tick)
			{
				break;
			}

			++msg_count;

			msg_ptr = cur_msg_ptr;
			// ------------------ begin -----------------//

			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(cur_msg_ptr->session_id));

			switch (packet->msg_id)
			{
			case NCI_Connect:
				{
					if (false == m_tag)
					{
						m_session_id = cur_msg_ptr->session_id;
						m_tag = true;
					}

					// std::cout << "connect create!" << std::endl;
				}
				break;
			case NCI_Disconnect:
				{
					// std::cout << "connect close!" << std::endl;
				}
				break;
			case NCC_Chat:
				{
					const tagNCC_Chat* msg_data_ptr = static_cast<const tagNCC_Chat*>(packet);

					char buff[256] = {0};
					memcpy(buff, msg_data_ptr->msg, msg_data_ptr->len);
					std::string str(buff);

					std::cout << msg_data_ptr->client_id << ": " << str << std::endl;

					// msg_data_ptr->session_id= 0;

					tagNCS_Chat* rep_ptr = g_ncserver.alloc_msg<tagNCS_Chat>();

					if (rep_ptr)
					{
						str += ", nexus network test, replay send!";
						rep_ptr->len = uint16(str.length());
						memcpy(rep_ptr->msg, str.c_str(), str.length());

						g_ncserver.send(cur_msg_ptr->session_id, NS2MSG(rep_ptr), sizeof(tagNCS_Chat));
					}
				}
				break;
			case NCC_Request:
				{
					tagNCC_Request* msg_data_ptr = (tagNCC_Request*)packet;

					//std::cout << msg_data_ptr->client_id << ": msg size = " << sizeof(tagNCC_Request) << std::endl;
					//std::cout << msg_data_ptr->time_stamp << std::endl;
					//std::cout << msg_data_ptr->type_i64 << std::endl;
					//std::cout << msg_data_ptr->type_i32 << std::endl;
					//std::cout << msg_data_ptr->type_i16 << std::endl;
					//// std::cout << msg_data_ptr->type_i8 << std::endl;
					//std::cout << msg_data_ptr->type_ui64 << std::endl;
					//std::cout << msg_data_ptr->type_ui32 << std::endl;
					//std::cout << msg_data_ptr->type_ui16 << std::endl;
					//// std::cout << msg_data_ptr->type_ui8 << std::endl;

					static uint32 i = 0;

					// 消息校验
					if (i != msg_data_ptr->type_ui32)
					{
						g_nperfmon.msg_error();
					}
					++i;

					tagNCS_Reply* rep_ptr = g_ncserver.alloc_msg<tagNCS_Reply>();

					if (rep_ptr)
					{
						rep_ptr->time_stamp	= msg_data_ptr->time_stamp;
						rep_ptr->type_i64	= msg_data_ptr->type_i64;
						rep_ptr->type_i32	= msg_data_ptr->type_i32;
						rep_ptr->type_i16	= msg_data_ptr->type_i16;
						rep_ptr->type_i8	= msg_data_ptr->type_i8;
						rep_ptr->type_ui64	= msg_data_ptr->type_ui64;
						rep_ptr->type_ui32	= msg_data_ptr->type_ui32;
						rep_ptr->type_ui16	= msg_data_ptr->type_ui16; 
						rep_ptr->type_ui8	= msg_data_ptr->type_ui8;

						// g_ncserver.send(m_session_id, NS2MSG(rep_ptr), sizeof(tagNCS_Reply));

						g_nperfmon.send(sizeof(tagNCS_Reply));

						if (false == g_ncserver.send(cur_msg_ptr->session_id, NS2MSG(rep_ptr), sizeof(tagNCS_Reply)))
						{
							g_nperfmon.send_error();
					}
				}
				}
				break;
			case NCC_StressTestMove:
				{
					tagNCC_StressTestMove* msg_data_ptr = (tagNCC_StressTestMove*)packet;

					uint32 rand_ = rand();

					tagNCS_StressTestMove msg;
					msg.dir			= msg_data_ptr->dir;
					msg.src			= msg_data_ptr->src;
					msg.flag		= msg_data_ptr->flag;
					msg.flag2		= msg_data_ptr->flag2;
					msg.time_stamp	= msg_data_ptr->time_stamp;

					msg.session_num = (rand_ % 60);

					uint32 msg_size = sizeof(msg) + msg.session_num * 4;

					g_nperfmon.send(msg_size);

					if (false == g_ncserver.send_msg(cur_msg_ptr->session_id, &msg, msg_size))
					{
						g_nperfmon.send_error();
					}

					//if (rand_ % 2 == 0)
					//{
					//	tagNCS_Reply rep;

					//	g_nperfmon.send(sizeof(rep));

					//	if (false == g_ncserver.send_msg(cur_msg_ptr->session_id, &rep, sizeof(rep)))
					//	{
					//		g_nperfmon.send_error();
					//	}
					//}
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

	nworld g_nworld;

} // namespace nexus