/**
 *	nexus ngame_define - nspell_data
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NSPELL_DATA_H_
#define _NDEF_GAME_NSPELL_DATA_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

	const uint32 MAX_SPELL_NUM			= 1024;		// 技能的最大数目
	const uint32 MAX_AURA_NUM			= 256;		// buff的最大数目
	const uint32 MAX_CD_NUM				= 50;		// cd保存的最大数目, 是否存储cd, 当前cd大于500ms

	enum ESpellCategoryCDType
	{
		ESCCDT_Null						= 0,
		ESCCDT_GlobalCD					= 1,		// 全局公共cd
		ESCCDT_Max,
	};

	struct nspell_cool_dowm
	{
		uint32					spell_id;			// 技能模板id
		uint32					category;			// cd公共类型
		uint64					cd_time;			// 到期时间time_t
	};

	// buff实例数据(size = 10)
	struct naura_data
	{
		uint32					info_id;			// buff模板id
		uint8					slot;				// buff槽位, guid
		uint8					stack_count;		// 当前叠加次数
		uint32					remain_time;		// 剩余时间(单位秒1/1000s)
		uint32					max_duration;		// 总时间(单位秒1/1000s)
	};

	struct naura_data_db : public naura_data
	{
		uint64					caster_id;			// 释放者id
		int32					amount;				// 数量统计, 作用次数, 吸收的伤害等等
		uint32					spell_id;			// 触发该buff的技能id, 没有为0
		uint8					index;				// 技能效果索引
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NSPELL_DATA_H_
