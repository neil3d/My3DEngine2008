#include "StdAfx.h"
#include "client.h"
#include "client_session.h"
#include "msg_dispatch.h"

client::client()
{
}

client::~client()
{
}

client* client::instance()
{
	static client s_inst;
	return &s_inst;
}

void client::on_connect(uint32 error_code)
{
	switch (error_code)
	{
	case ECS_Connected:
		{
			client_session::instance()->set_status(ECSS_Connected);
			nLog_Info(_T("Nexus Dummy Client connect server OK ..."));
		}
		break;
	case ECS_ConnectTimeout:
		{
			nLog_Error(_T("Nexus Dummy Client connect to the server timeout ..."));
		}
		break;
	case ECS_ConnectAborted:
		{
			nLog_Error(_T("Nexus Dummy Client connect to the server abort ..."));
		}
		break;
	case ECS_Error:
		{
			nLog_Error(_T("Nexus Dummy Client connect to the server error ..."));
		}
		break;
	default:
		{
			nLog_Error(_T("Nexus Dummy Client connect to the server error ..."));
		}
		break;
	}
}

void client::on_datarecv(const void* data_ptr, uint32 size)
{
	// 将数据拷贝到消息队列
	if (false == g_msg_dispatch.push_msg(data_ptr, size))
	{
		nLog_Error(_T("message queue push error!"));
	}
}

void client::on_disconnect()
{
	client_session::instance()->set_status(ECSS_Disconnect);
}
