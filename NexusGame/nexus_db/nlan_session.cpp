#include "nlan_session.h"
#include "nlan_client.h"
#include "nenums.h"
#include "nlog_mt.h"
#include "nmsg_dispatch.h"

#include "nmsg_cluster.h"
#include "nmsg_world_db.h"

#include "nhandle_load.h"


namespace nexus {

	nlan_session::nlan_session() : m_session_id(0XFFFFFFFF), m_server_type(EST_None), m_server_id(0), m_status(ELSS_None), m_address(""), 
		m_port(0), m_msg_queue(10000)
	{
	}

	nlan_session::~nlan_session()
	{
	}

	bool nlan_session::init(uint32 session_id, uint32 server_type, const std::string& address, uint16 port)
	{
		m_session_id	= session_id;
		m_server_type	= server_type;
		m_status		= ELSS_Connected;
		m_address		= address;
		m_port			= port;
		return true;
	}

	// destroy 要进行该类所有数据的复原，因为要归还到池里
	void nlan_session::destroy()
	{
		m_session_id	= 0XFFFFFFFF;
		m_server_type	= EST_None;
		m_status		= ELSS_Closed;
		m_address		= "";
		m_port			= 0;
	}

	void nlan_session::update(uint32 elapse)
	{
		_handle_msg();
	}

	bool nlan_session::send_message(const void* msg_ptr, uint32 size)
	{
		return g_lan_client.send_msg(m_session_id, msg_ptr, size);
	}

	void nlan_session::_handle_msg()
	{
		nmsg_handler* handler_ptr = NULL;
		nmessage* msg_ptr = m_msg_queue.dequeue();
		nmessage* temp_msg_ptr = NULL;

		while (msg_ptr)
		{
			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));
			handler_ptr = g_msg_dispatch.get_msg_handler(packet->msg_id);

			if (!handler_ptr || !handler_ptr->handler)
			{
				NLOG_ERROR(_T("get msg handler error, msg_id = %u, client_id = %lu"), packet->msg_id, packet->client_id);
			}
			else
			{
				++handler_ptr->handle_count;

				// TODO: D. Zhao状态判断
				(this->*(handler_ptr->handler))(packet);
			}
			// 注意msg的释放
			temp_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;
			temp_msg_ptr->free();
		}
	}

	void nlan_session::handle_reply_login(const nmsg_base* packet)
	{
		const tagMsg_Reply_Login* data_ptr = reinterpret_cast<const tagMsg_Reply_Login*>(packet);

		if (EError_Success == data_ptr->error_code)
		{
			set_status(ELSS_Logined);
			NLOG_INFO(_T("login server sucess, server_type = %u"), get_type());
		}
		else
		{
			NLOG_ERROR(_T("login server errror, server_type = %u"), get_type());
		}
	}

} // namespace nexus
