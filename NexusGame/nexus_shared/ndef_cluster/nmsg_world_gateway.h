/**
 *	nexus ndef_cluster - nmsg_world_gateway.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_WORLD_GATEWAY_H_
#define _NDEF_CLUSTER_NMSG_WORLD_GATEWAY_H_

#include "nmsg_base_id.h"
#include "nprotocol_base.h"
#include "nobject_define.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, 1701 - 1750
	 */
	enum
	{
		Msg_World_Gateway_Begin		= Msg_World_Gateway + 1,

		// login
		Msg_G2W_Server_Info			= 1702,


		Msg_G2W_Broadcast			= 1749,
		Msg_World_Gateway_End		= 1750,
	};

	CMD_START(Msg_G2W_Server_Info)
		uint32		server_id;
		uint32		login_ip;
		uint32		bg_ip;
		uint16		login_port;
		uint16		bg_port;
	CMD_END

	struct tagMsg_G2W_Broadcast : public nmsg_base
	{
		char data[1024 * 13];

		tagMsg_G2W_Broadcast()
		{
			memset(this, 0, sizeof(*this));
			msg_id = Msg_G2W_Broadcast;
		}

		bool push_msg(const void* msg_ptr, uint32 size)
		{
			if (size > 1024)
			{
				return false;
			}

			memcpy(this, msg_ptr, size);
			*(uint32*)(&client_id) = size;
			*(uint16*)((char*)this + size) = 0;
			return true;
		}

		void push_client(uint32 client_id)
		{
			*(uint32*)((char*)this + get_packet_size()) = client_id;
			*(uint16*)((char*)this + get_msg_size()) = get_client_num() + 1;
		}

		void set_original_msg_id(uint16 msg_id)
		{
			*((uint16*)(&client_id) + 2) = msg_id;
		}

		uint32 get_msg_size() const
		{
			return *(uint32*)(&client_id);
		}

		uint32 get_packet_size() const
		{
			return get_msg_size() + sizeof(uint16) + get_client_num() * sizeof(uint32);
		}

		uint16 get_original_msg_id() const
		{
			return *((uint16*)(&client_id) + 2);
		}

		uint16 get_client_num() const
		{
			return *(uint16*)((char*)this + get_msg_size());
		}

		uint32 get_client_id(uint16 index) const
		{
			return *(uint32*)((char*)this + get_msg_size() + sizeof(uint16) + index * sizeof(uint32));
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_WORLD_GATEWAY_H_