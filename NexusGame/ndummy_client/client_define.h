#pragma once

#include "ncommon.h"
#include "nsafe_mem_pool.h"
#include "nmsg_base_id.h"

using namespace nexus;

class client_session;

#pragma pack(push,1)

struct client_msg
{
	nsafe_mem_pool*		pool_ptr;
	client_msg*			next_ptr;
	uint32				size;
	char				data[sizeof(uint32)];

	client_msg() : pool_ptr(NULL), next_ptr(NULL), size(0) {}
};

typedef void (client_session::*client_handler_type)(const nmsg_base* msg_ptr);

struct client_msg_handler
{
	tstring					msg_name;
	uint32					status;
	uint32					handle_count;
	client_handler_type		handler;

	client_msg_handler() : msg_name(_T("")), status(0), handle_count(0), handler(NULL) {}
};

#pragma pack(pop)
