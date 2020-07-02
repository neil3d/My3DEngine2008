/**
 *	nexus ndef_cluster - nmsg_world_bg.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_WORLD_BG_H_
#define _NDEF_CLUSTER_NMSG_WORLD_BG_H_

#include "nmsg_base_id.h"
#include "nprotocol_base.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, bg id 1601-1700
	 */
	enum
	{
		Msg_World_BG_Begin		= Msg_World_BG + 1,




		Msg_World_BG_End		= 1700,
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_WORLD_BG_H_
