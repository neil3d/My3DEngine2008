/**
 *	nexus ndef_cluster - nmsg_world_login.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_WORLD_LOGIN_H_
#define _NDEF_CLUSTER_NMSG_WORLD_LOGIN_H_

#include "nmsg_base_id.h"
#include "nprotocol_base.h"
#include "nenums.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, 1751 - 1950
	 */
	enum
	{
		Msg_World_Login_Begin		= Msg_World_Login + 1,

		Msg_W2L_World_Request_Login,
		Msg_L2W_Player_Login,

		Msg_World_Login_End			= 1950,
	};

	CMD_START(Msg_W2L_World_Request_Login)
		uint32			server_id;
		nchar			world_name[MAX_SERVER_NAME_LEN];
	CMD_END

	CMD_START(Msg_L2W_Player_Login)
		uint32			account_id;
		uint32			online_time;
		uint8			privilege;
		nchar			account[32];
		uint64			prelogout_time;
		uint32			prelogin_ip;
		uint32			login_ip;
		uint16			login_port;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_WORLD_LOGIN_H_