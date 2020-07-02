#pragma once
#include "NEngine.h"
#include "NFileSystem.h"
#include "NAttDataHelper.h"
#include "nspell_define.h"
#include "ngameframework.h"
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

namespace NexusEngine
{
	using namespace NexusEngineExtension;
	using namespace nexus;
	using namespace gameframework;

	/**
	* class NSpellTalentTable
	* 对nexus::nspell_talent_table封装
	* 为CLR提供方面Native struct接口
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellTalentTable, nspell_talent_table, nspell_talent_table_dictionary);
		// 职业
		DECLARE_ATTINFO_PROPERTY(UInt32,Class,NativePtr->class_,"职业");

		// 人物等级
		DECLARE_ATTINFO_PROPERTY(UInt32,Level,NativePtr->level,"人物等级");

		// 已加技能点
		DECLARE_ATTINFO_PROPERTY(UInt32,Point,NativePtr->point,"已加技能点");

		// 消耗的游戏币
		DECLARE_ATTINFO_PROPERTY(UInt32,Money,NativePtr->money,"消耗游戏币");

		// 所需声望
		DECLARE_ATTINFO_PROPERTY(UInt32,ReputationFaction,NativePtr->reputation_faction,"所需声望");

		// 所需声望等级
		DECLARE_ATTINFO_PROPERTY(UInt32,ReputationRank,NativePtr->reputation_rank,"所需声望等级");
		
		// 所需技能树节点
		DECLARE_ATTINFO_NAME_PROPERTY(DependsOn,NativePtr->depends_on,"所需技能树节点");
		
		// 所需加点等级
		DECLARE_ATTINFO_PROPERTY(UInt32,DependsOnRank,NativePtr->depends_on_rank,"所需加点等级");
		
		// 所需技能树节点
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_0,NativePtr->spell_id[0],"技能0");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_1,NativePtr->spell_id[1],"技能1");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_2,NativePtr->spell_id[2],"技能2");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_3,NativePtr->spell_id[3],"技能3");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_4,NativePtr->spell_id[4],"技能4");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_5,NativePtr->spell_id[5],"技能5");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_6,NativePtr->spell_id[6],"技能6");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_7,NativePtr->spell_id[7],"技能7");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_8,NativePtr->spell_id[8],"技能8");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellId_9,NativePtr->spell_id[9],"技能9");

	DECLARE_ATT_INFO_CLASS_END();

}
