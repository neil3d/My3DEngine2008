/**
 *	nexus ndef_cluster - nmsg_cluster.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_CLUSTER_H_
#define _NDEF_CLUSTER_NMSG_CLUSTER_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, 服务器间通用协议,id 1951-2000
	 */
	enum
	{
		Msg_Cluster_Begin		= Msg_Cluster + 1,

		// login
		Msg_Request_Login		= 1952,
		Msg_Reply_Login			= 1953,
		Msg_Player_Logout		= 1954,


		Msg_Cluster_End			= 2000,
	};

	CMD_START(Msg_Request_Login)
		uint32 server_type;
		uint32 server_id;
	CMD_END

	CMD_START(Msg_Reply_Login)
		int32 error_code;
	CMD_END

	CMD_START(Msg_Player_Logout)
		uint32 account_id;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_CLUSTER_H_
