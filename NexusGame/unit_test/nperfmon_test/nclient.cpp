#include "nclient.h"
#include "nworld_session.h"
#include "nclient_session.h"
#include "nlog_mt.h"
#include "ndefine.h"

namespace nexus {

	nclient::nclient()
	{
	}

	nclient::~nclient()
	{
	}

	void nclient::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		nmessage* msg_ptr = make_connect_msg(session_id, param1, param2);

		if (NULL == msg_ptr)
		{
			NLOG_ERROR(_T("alloc msg error, session_id = %u"), session_id);
			return;
		}

		g_world_session.push_msg(msg_ptr);
	}

	void nclient::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		g_world_session.push_msg(msg_ptr);
	}

	void nclient::on_disconnect(uint32 session_id)
	{
		nmessage* msg_ptr = make_disconnect_msg(session_id);

		if (NULL == msg_ptr)
		{
			NLOG_ERROR(_T("alloc msg error, session_id = %u"), session_id);
			return;
		}

		g_world_session.push_msg(msg_ptr);
	}

	bool nclient::send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
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

	nmessage* nclient::make_connect_msg(uint32 session_id, uint32 param1, uint32 param2)
	{
		nmessage* nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + sizeof(tagCI_Connect)));

		if (NULL == nmsg_ptr)
		{
			return NULL;
		}

		nmsg_ptr->pool_ptr		= &m_pool;
		nmsg_ptr->next_ptr		= NULL;
		nmsg_ptr->reference		= 1;				// 引用计数1的设置，在调用send之前
		nmsg_ptr->size			= sizeof(tagCI_Connect);
		nmsg_ptr->session_id	= session_id;
		nmsg_ptr->server_id		= 0;

		tagCI_Connect data;
		data.session_id	= session_id;
		data.param1		= param1;
		data.param2		= param2;
		// 拷贝数据，注意起始地址
		memcpy(nmsg_ptr->buffer, &data, sizeof(data));
		return nmsg_ptr;
	}

	nmessage* nclient::make_disconnect_msg(uint32 session_id)
	{
		nmessage* nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + sizeof(tagCI_DisConnect)));

		if (NULL == nmsg_ptr)
		{
			return NULL;
		}

		nmsg_ptr->pool_ptr		= &m_pool;
		nmsg_ptr->next_ptr		= NULL;
		nmsg_ptr->reference		= 1;				// 引用计数1的设置，在调用send之前
		nmsg_ptr->size			= sizeof(tagCI_DisConnect);
		nmsg_ptr->session_id	= session_id;
		nmsg_ptr->server_id		= 0;

		tagCI_DisConnect data;
		data.session_id	= session_id;
		// 拷贝数据，注意起始地址
		memcpy(nmsg_ptr->buffer, &data, sizeof(data));
		return nmsg_ptr;
	}

	nmessage* nclient::make_command_msg(nchar* command_str)
	{
		nmessage* nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + sizeof(tagCI_Command)));

		if (NULL == nmsg_ptr)
		{
			return NULL;
		}

		nmsg_ptr->pool_ptr		= &m_pool;
		nmsg_ptr->next_ptr		= NULL;
		nmsg_ptr->reference		= 1;				// 引用计数1的设置，在调用send之前
		nmsg_ptr->size			= sizeof(tagCI_Command);
		nmsg_ptr->session_id	= 0;
		nmsg_ptr->server_id		= 0;

		tagCI_Command data;
		memcpy(data.cmd, command_str, sizeof(nchar) * _tcsclen(command_str));
		// 拷贝数据，注意起始地址
		memcpy(nmsg_ptr->buffer, &data, sizeof(data));
		return nmsg_ptr;
	}

	nclient g_client;

} // namespace nexus
