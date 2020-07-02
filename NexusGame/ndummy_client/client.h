#pragma once

#include "nnetwork.h"

/**
 *	客户端数据传输类
 */
class client : public nexus::ntcp_client
{
public:
	client();
	virtual ~client();

	static client* instance();

	virtual void on_connect(uint32 error_code);
	virtual void on_datarecv(const void* msg_ptr, uint32 size);
	virtual void on_disconnect();
};
