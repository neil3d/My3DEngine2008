/**
*	nexus ndef_levelup
* 角色升级数据模板定义
*/

#ifndef _NDEF_GAME_NDEF_LEVELUP_H_
#define _NDEF_GAME_NDEF_LEVELUP_H_

#include "ntype_def.h"
#include "ndef_base.h"

namespace nexus {

#pragma pack(push,1)

	// 角色升级表数据
	// id为角色的等级
	struct levelup_info : public att_info_base
	{
		uint32			exp;					//下一级需要的经验

		levelup_info(void)
			: exp(0xFFFFFFFF)
		{
		}

		explicit levelup_info(const nstring& id_str) 
			: att_info_base(id_str),exp(0xFFFFFFFF)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nSERIALIZE(ar, exp);
		}
	};

	//-- 
	SERIALIZE_ATT_INFO(levelup_info);


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NDEF_LEVELUP_H_
