#pragma once

#include "ncommon.h"
#include "nsafe_mem_pool.h"
#include "nmsg_base_id.h"

#pragma pack(push,1)

namespace nexus
{
	struct client_msg
	{
		nsafe_mem_pool*		pool_ptr;
		client_msg*			next_ptr;
		uint32				size;
		char				data[sizeof(uint32)];

		client_msg() : pool_ptr(NULL), next_ptr(NULL), size(0) {}
	};
}

#pragma pack(pop)
