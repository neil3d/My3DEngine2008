/**
 *	nexus db - ncache_utility
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCACHE_UTILITY_H_
#define _NDB_NCACHE_UTILITY_H_

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>


#include "ncommon.h"
#include "nitem_define.h"
#include "nlogin_define.h"
#include "nobject_pool.h"
#include "timer/ntimestamp.h"

//检测无效指针
#define UNVALID_POINT(ptr) (NULL==(ptr))

//检测有效指针
#define VALID_POINT(ptr) (return NULL!=(ptr))

namespace nexus{
#pragma pack(push,1)

	struct data_base
	{
		bool	change;	// 表明该结构体数据是否发生改变
	};

	/**
	 *	记录玩家的账号和所属的游戏世界信息
	 *	@remark : 该结构体共 (40 + 16 + 20 + 16) 字节
	 */
	struct ncharacter_cache : public data_base
	{
		//账号、角色相关(40 byte)
		uint32	account_id;				// 账号ID
		uint64	role_id;				// 角色ID
		nchar	name[MAX_ACCOUNT_LEN];	// 角色名称

		//职业、角色属性相关(16字节)
		uint8	sex;					// 性别
		uint8	race;					// 种族
		uint8	class_type;				// 职业
		uint8	level;					// 当前等级
		uint32	cur_exp;				// 当前拥有的经验值
		uint32	hp;						// 血量
		uint32	mp;						// 真气量

		//位置相关(20字节)
		uint16	map_id;					// 角色所在地图ID
		float	x;						// X坐标
		float	y;						// y坐标
		float	z;						// z坐标
		float	orient;					// 朝向

		//时间相关(16字节)
		uint32	create_time; 			// 角色创建时间
		uint32	login_time;				// 上线时间
		uint32	logout_time;			// 下线时间
		uint32	online_time;			// 总在线时间(单位:秒)
		
		ncharacter_cache()
		{
			memset(this, 0, sizeof(ncharacter_cache));
			name[0] = '\0';
		}

		static ncharacter_cache* alloc_character_data(void);
		static void free_character_data(ncharacter_cache* character_cache_ptr);
	};

	struct nitem_cache : public data_base
	{
		nitem_data	item;
		static nitem_cache* alloc_item_data(void);
		static void free_item_data(nitem_cache* item_cache_ptr);
	};

	struct nequip_cache : public data_base
	{
		nequip_data	equip;

		nequip_cache()
		{
			memset(this, 0, sizeof(nequip_cache));
		}

		static nequip_cache* alloc_equip_data(void);
		static void free_equip_data(nequip_cache* equip_cache_ptr);
	};

	struct nskill_cache : public data_base
	{
		uint32	id;
		uint32	cd;

		nskill_cache()
		{
			id = 0;
			cd = 0;
		}

		static nskill_cache* alloc_skill_data(void);
		static void free_skill_data(nskill_cache* skill_cache_ptr);
	};

	struct nbuff_cache : public data_base
	{
	};

	typedef	boost::unordered_map<uint32, uint32>			Uint32Map;
	typedef	Uint32Map::iterator								Uint32MapIt;


	typedef	boost::unordered_set<uint64>					RoleSet;			//一个账号创建的不同角色对象
	typedef	RoleSet::iterator								RoleSetIt;


	typedef	boost::unordered_map<uint32, RoleSet>			AccountRoleMap;		//[key:account_id   value:role_ids]
	typedef	AccountRoleMap::iterator						AccountRoleMapIt;


	typedef	boost::unordered_map<uint64, ncharacter_cache*>	CharacterMap;		//[key:role_id		value:ncharacter_cache]
	typedef	CharacterMap::iterator							CharacterMapIt;


	typedef	boost::unordered_map<uint64, nitem_cache*>		ItemMap;			//[key:guid			value:nitem_cache*]
	typedef	ItemMap::iterator								ItemMapIt;

	typedef	boost::unordered_map<uint64, nequip_cache*>		EquipMap;			//[key:guid			value:nequip_cache*]
	typedef	EquipMap::iterator								EquipMapIt;

	typedef	boost::unordered_map<uint32, nskill_cache*>		SkillMap;			//[key:skill_id		value:nskill_cache*]
	typedef	SkillMap::iterator								SkillMapIt;


	typedef std::list<uint64>								Uint64List;
	typedef	Uint64List::iterator							Uint64ListIt;


	//非线程安全的内存池
	extern nobject_pool<nnull_mutex>  g_pool;	//用于分配内存的内存池

	////线程安全的内存池
	//extern nobject_pool<nfast_mutex>  g_pool;	//用于分配内存的内存池


	//以下的所有宏定义用于测试时间相关的
	//#define	NSTORE_CENTER_CACHE_CHARACTER		//nstore_center::cache_character里面测试执行sql查询并返回结果集的时间
	//
	//#define	NSTORE_CENTER_CACHE_ITEM			//nstore_center::cache_item里面测试执行sql查询并返回结果集的时间
	//#define	NSTORE_CENTER_UPDATE_ITEM			//nstore_center::update_item里面测试执行sql的时间
	//
	//#define	NSTORE_CENTER_CACHE_EQUIP			//nstore_center::cache_equip里面测试执行sql查询并返回结果集的时间
	//#define	NSTORE_CENTER_UPDATE_EQUIP_SPRINTF	//nstore_center::update_equip里面测试拼写sql语句的时间
	//#define	NSTORE_CENTER_UPDATE_EQUIP			//nstore_center::update_equip里面测试执行sql的时间
	//
	//#define	NSTORE_CENTER_CACHE_SKILL			//nstore_center::cache_skill里面测试执行sql查询并返回结果集的时间
	//#define	NSTORE_CENTER_UPDATE_SKILL			//nstore_center::update_skill里面测试执行sql的时间

	//#define	NITEM_UNLOAD_CACHE					//nitem::unload_cache里存储角色所有物品的时间
	//#define	NEQUIP_UNLOAD_CACHE					//nequip::unload_cache里存储角色所有物品的时间
	//#define	NSKILL_UNLOAD_CACHE					//nskill::unload_cache里存储角色所有物品的时间

#pragma pack(pop)

} // namespace nexus

#endif	_NDB_NCACHE_UTILITY_H_