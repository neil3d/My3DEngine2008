#include "ncommon.h"

namespace nexus {

	enum EMapType
	{
		EMT_Common						= 0,					// 主世界地图
		EMT_Instance					= 1,					// 副本地图
		EMT_ForeverInstance				= 2,					// 新手村地图
		EMT_Battleground				= 3,					// 战场
		EMT_Arena						= 4                     // 竞技场
	};

	struct nmap_desc
	{
		uint32		map_id;
		uint32		instance_num;

		nmap_desc() : map_id(0), instance_num(0) {}
	};

	struct narea_trigger_info
	{
		uint32					info_id;						// id
		uint32					type;							// 类型
	};

	struct nmap_info
	{
		uint32					info_id;						// 地图逻辑id
		uint32					type;							// 地图类型
		tstring					name;							// 地图名称
		float					visible_distance;				// 玩家, 普通怪可视距离
		uint32					max_player;						// 最大玩家数目(不包括gm)
		uint32					born_pos_id[5];					// 出生点id
		uint32					relive_pos_id[5];				// 复活点id

		// 刷怪点信息
		// 触发器信息
		// 区域信息
	};


} // namespace nexus
