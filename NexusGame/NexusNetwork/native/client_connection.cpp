#include "StdAfx.h"
#include "client_connection.h"
#include "client_define.h"

namespace nexus
{
	client_connection::client_connection()
	{
	}

	client_connection::~client_connection()
	{
	}

	void client_connection::on_connect(uint32 error_code)
	{
		switch (error_code)
		{
		case ECS_Connected:
			{
				set_status(ECSS_Connected);

				msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
				for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
				{
					(*it)->on_connected();
				}

				nLog_Info(_T("Nexus Dummy Client connect server OK ..."));
			}
			break;
		case ECS_ConnectTimeout:
			{
				msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
				for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
				{
					(*it)->on_connect_timeout();
				}

				nLog_Error(_T("Nexus Dummy Client connect to the server timeout ..."));
			}
			break;
		case ECS_ConnectAborted:
			{
				msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
				for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
				{
					(*it)->on_connect_aborted();
				}

				nLog_Error(_T("Nexus Dummy Client connect to the server abort ..."));
			}
			break;
		case ECS_Error:
			{
				msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
				for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
				{
					(*it)->on_connect_error();
				}

				nLog_Error(_T("Nexus Dummy Client connect to the server error ..."));
			}
			break;
		default:
			{
				msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
				for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
				{
					(*it)->on_connect_error();
				}

				nLog_Error(_T("Nexus Dummy Client connect to the server error ..."));
			}
			break;
		}
	}

	void client_connection::on_datarecv(const void* data_ptr, uint32 size)
	{
		// 将数据拷贝到消息队列
		if (false == push_msg(data_ptr, size))
		{
			nLog_Error(_T("message queue push error!"));
		}
	}

	void client_connection::on_disconnect()
	{
		set_status(ECSS_Disconnect);

		msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
		for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
		{
			(*it)->on_disconnected();
		}
	}

	void client_connection::update(float delta_time)
	{
		client_msg* msg_ptr = m_msg_queue.dequeue();
		client_msg* cur_msg_ptr = NULL;

		while (msg_ptr)
		{
			on_handle_msg(msg_ptr);

			cur_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;
			cur_msg_ptr->pool_ptr->free(cur_msg_ptr);
		}
	}


	void client_connection::on_handle_msg( client_msg* msg_ptr )
	{
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(msg_ptr->data);

		// 校验msg_id
		if (packet->msg_id >= Msg_Max)
		{
			nLog_Info(_T("error msg id"));
			return;
		}

		msg_listener_list::const_iterator itEnd = m_msg_listeners.end();
		for (msg_listener_list::const_iterator it = m_msg_listeners.begin(); it != itEnd; ++it)
		{
			(*it)->on_msg(packet->msg_id, packet);
		}
	}
}
