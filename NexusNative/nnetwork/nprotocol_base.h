/**
 *	nexus network - nprotocol_base.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NPROTOCOL_BASE_H_
#define _NNETWORK_NPROTOCOL_BASE_H_

#include "../ncore/ntype_def.h"

#include <string>

#pragma pack(push,1)

namespace nexus {

/**
 *	网络消息基本结构, 服务器需要define _NSERVER
 */
#ifdef _NSERVER
	struct nmsg_base
	{
		uint64		client_id;
		uint16		msg_id;
	};
#else
	struct nmsg_base
	{
		uint16		msg_id;
	};
#endif

/**
 *	方便网络消息的定义
 */
#ifndef CMD_START
#	define CMD_START(name)	struct tag##name : public nmsg_base { tag##name() { memset(this, 0, sizeof(*this)); msg_id = (##name); }
#	define CMD_END	};
#endif

} // namespace nexus

#pragma pack(pop)

#endif // _NNETWORK_NPROTOCOL_BASE_H_
