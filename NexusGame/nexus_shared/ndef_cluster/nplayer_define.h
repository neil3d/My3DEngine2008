/**
 *	nexus ndef_cluster - nplayer_define.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NPLAYER_DEFINE_H_
#define _NDEF_CLUSTER_NPLAYER_DEFINE_H_

#include "nobject_define.h"

namespace nexus {

#pragma pack(push,1)

	// 离线玩家数据
	struct nplayer_info
	{
		uint64			player_id;						// 8 ID
		uint32			name_crc;						// 4 名字CRC
		uint32			account_id;						// 4 账户ID
		nchar			name[MAX_NAME_LEN];				// 16 名字
		uint32			map_id;							// 4 所在地图
		uint64			last_online_time;				// 8 最后上线时间
		uint16			flag;							// 2 职业，等级，是否在线
	};




#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NPLAYER_DEFINE_H_
