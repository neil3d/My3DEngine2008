/**
 *	nexus db - nstore_center_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NSTORE_CENTER_RESULT_H_
#define _NDB_NSTORE_CENTER_RESULT_H_

namespace nexus{

//最低效
void _cache_character_result(ndb_query_result& query_result, ncharacter_cache* data_ptr)
{
	data_ptr->account_id	= query_result.get_real_value<uint32>("account_id");
	data_ptr->role_id		= query_result.get_real_value<uint32>("role_id");
	query_result.get_real_value_str("name", data_ptr->name, MAX_ACCOUNT_LEN);

	data_ptr->sex			= query_result.get_real_value_byte("sex");
	data_ptr->race			= query_result.get_real_value_byte("race");
	data_ptr->class_type	= query_result.get_real_value_byte("class_type");
	data_ptr->level			= query_result.get_real_value_byte("level");
	data_ptr->cur_exp		= query_result.get_real_value<uint32>("cur_exp");
	data_ptr->hp			= query_result.get_real_value<uint32>("hp");
	data_ptr->mp			= query_result.get_real_value<uint32>("mp");

	data_ptr->map_id		= query_result.get_real_value<uint32>("map_id");
	data_ptr->x				= query_result.get_real_value<float>("x");
	data_ptr->y				= query_result.get_real_value<float>("y");
	data_ptr->z				= query_result.get_real_value<float>("z");
	data_ptr->orient		= query_result.get_real_value<float>("orient");

	data_ptr->create_time	= query_result.get_real_value<uint32>("create_time");
	data_ptr->login_time	= query_result.get_real_value<uint32>("login_time");
	data_ptr->logout_time	= query_result.get_real_value<uint32>("logout_time");
	data_ptr->online_time	= query_result.get_real_value<uint32>("online_time");
}

//低效
void _cache_character_result_index(ndb_query_result& query_result, ncharacter_cache* data_ptr)
{
	data_ptr->account_id	= query_result.get_real_value<uint32>((uint32)0);
	data_ptr->role_id		= query_result.get_real_value<uint32>(1);
	query_result.get_real_value_str(2, data_ptr->name, MAX_ACCOUNT_LEN);

	data_ptr->sex			= query_result.get_real_value_byte(3);
	data_ptr->race			= query_result.get_real_value_byte(4);
	data_ptr->class_type	= query_result.get_real_value_byte(5);
	data_ptr->level			= query_result.get_real_value_byte(6);
	data_ptr->cur_exp		= query_result.get_real_value<uint32>(7);
	data_ptr->hp			= query_result.get_real_value<uint32>(8);
	data_ptr->mp			= query_result.get_real_value<uint32>(9);

	data_ptr->map_id		= query_result.get_real_value<uint32>(10);
	data_ptr->x				= query_result.get_real_value<float>(11);
	data_ptr->y				= query_result.get_real_value<float>(12);
	data_ptr->z				= query_result.get_real_value<float>(13);
	data_ptr->orient		= query_result.get_real_value<float>(14);

	data_ptr->create_time	= query_result.get_real_value<uint32>(15);
	data_ptr->login_time	= query_result.get_real_value<uint32>(16);
	data_ptr->logout_time	= query_result.get_real_value<uint32>(17);
	data_ptr->online_time	= query_result.get_real_value<uint32>(18);
}

//高效
void _cache_character_result_atoi(ndb_query_result& query_result, ncharacter_cache* data_ptr)
{
	data_ptr->account_id	= query_result.get_uint32((uint32)0);
	data_ptr->role_id		= query_result.get_uint32(1);
	query_result.get_real_value_str(2, data_ptr->name, MAX_ACCOUNT_LEN);

	data_ptr->sex			= query_result.get_uint8(3);
	data_ptr->race			= query_result.get_uint8(4);
	data_ptr->class_type	= query_result.get_uint8(5);
	data_ptr->level			= query_result.get_uint8(6);
	data_ptr->cur_exp		= query_result.get_uint32(7);
	data_ptr->hp			= query_result.get_uint32(8);
	data_ptr->mp			= query_result.get_uint32(9);

	data_ptr->map_id		= query_result.get_uint32(10);
	data_ptr->x				= query_result.get_float(11);
	data_ptr->y				= query_result.get_float(12);
	data_ptr->z				= query_result.get_float(13);
	data_ptr->orient		= query_result.get_float(14);

	data_ptr->create_time	= query_result.get_uint32(15);
	data_ptr->login_time	= query_result.get_uint32(16);
	data_ptr->logout_time	= query_result.get_uint32(17);
	data_ptr->online_time	= query_result.get_uint32(18);
}

//最低效
void _cache_item_result(ndb_query_result& query_result, nitem_cache* data_ptr)
{
	data_ptr->item.guid		= query_result.get_real_value<uint64>("guid");
	data_ptr->item.info_id	= query_result.get_real_value<uint32>("info_id");
	data_ptr->item.count	= query_result.get_real_value<uint16>("count");
	data_ptr->item.slot		= query_result.get_real_value<uint16>("slot");
	data_ptr->item.bag		= query_result.get_real_value_byte("bag");
	data_ptr->item.flag		= query_result.get_real_value_byte("flag");
}

//低效
void _cache_item_result_index(ndb_query_result& query_result, nitem_cache* data_ptr)
{
	data_ptr->item.guid		= query_result.get_real_value<uint64>((uint32)0);
	data_ptr->item.info_id	= query_result.get_real_value<uint32>(1);
	data_ptr->item.count	= query_result.get_real_value<uint16>(2);
	data_ptr->item.slot		= query_result.get_real_value<uint16>(3);
	data_ptr->item.bag		= query_result.get_real_value_byte(4);
	data_ptr->item.flag		= query_result.get_real_value_byte(5);
}

//高效
void _cache_item_result_atoi(ndb_query_result& query_result, nitem_cache* data_ptr)
{
	data_ptr->item.guid		= query_result.get_uint64((uint32)0);
	data_ptr->item.info_id	= query_result.get_uint32(1);
	data_ptr->item.count	= query_result.get_uint16(2);
	data_ptr->item.slot		= query_result.get_uint16(3);
	data_ptr->item.bag		= query_result.get_uint8(4);
	data_ptr->item.flag		= query_result.get_int8(5);
}

//最低效
void _cache_equip_result(ndb_query_result& query_result, nequip_cache* data_ptr)
{
	data_ptr->equip.guid			= query_result.get_real_value<uint64>("guid");
	data_ptr->equip.info_id 		= query_result.get_real_value<uint32>("info_id");
	data_ptr->equip.count			= query_result.get_real_value<uint16>("count");
	data_ptr->equip.slot			= query_result.get_real_value<uint16>("slot");
	data_ptr->equip.bag				= query_result.get_real_value_byte("bag");
	data_ptr->equip.flag			= query_result.get_real_value_byte("flag");

	data_ptr->equip.quality			= query_result.get_real_value_byte("quality");
	data_ptr->equip.level			= query_result.get_real_value_byte("level");
	data_ptr->equip.durability		= query_result.get_real_value<uint32>("durability");
	data_ptr->equip.weight			= query_result.get_real_value<uint16>("weight");
	data_ptr->equip.sharp			= query_result.get_real_value<uint16>("sharp");
	data_ptr->equip.solid			= query_result.get_real_value<uint16>("solid");
	data_ptr->equip.tenacity		= query_result.get_real_value<uint16>("tenacity");;

	data_ptr->equip.stat_type[0]	= query_result.get_real_value_byte("stat_type0");
	data_ptr->equip.stat_type[1]	= query_result.get_real_value_byte("stat_type1");
	data_ptr->equip.stat_type[2]	= query_result.get_real_value_byte("stat_type2");
	data_ptr->equip.stat_type[3]	= query_result.get_real_value_byte("stat_type3");
	data_ptr->equip.stat_type[4]	= query_result.get_real_value_byte("stat_type4");
	data_ptr->equip.stat_type[5]	= query_result.get_real_value_byte("stat_type5");
	data_ptr->equip.stat_type[6]	= query_result.get_real_value_byte("stat_type6");
	data_ptr->equip.stat_type[7]	= query_result.get_real_value_byte("stat_type7");

	data_ptr->equip.stat_value[0]	= query_result.get_real_value<uint16>("stat_value0");
	data_ptr->equip.stat_value[1]	= query_result.get_real_value<uint16>("stat_value1");
	data_ptr->equip.stat_value[2]	= query_result.get_real_value<uint16>("stat_value2");
	data_ptr->equip.stat_value[3]	= query_result.get_real_value<uint16>("stat_value3");
	data_ptr->equip.stat_value[4]	= query_result.get_real_value<uint16>("stat_value4");
	data_ptr->equip.stat_value[5]	= query_result.get_real_value<uint16>("stat_value5");
	data_ptr->equip.stat_value[6]	= query_result.get_real_value<uint16>("stat_value6");
	data_ptr->equip.stat_value[7]	= query_result.get_real_value<uint16>("stat_value7");

	data_ptr->equip.gem[0]			= query_result.get_real_value<uint16>("gem0");
	data_ptr->equip.gem[1]			= query_result.get_real_value<uint16>("gem1");
	data_ptr->equip.gem[2]			= query_result.get_real_value<uint16>("gem2");
	data_ptr->equip.gem[3]			= query_result.get_real_value<uint16>("gem3");
	data_ptr->equip.gem[4]			= query_result.get_real_value<uint16>("gem4");
	data_ptr->equip.gem[5]			= query_result.get_real_value<uint16>("gem5");

	data_ptr->equip.spell[0]		= query_result.get_real_value<uint16>("spell0");
	data_ptr->equip.spell[1]		= query_result.get_real_value<uint16>("spell1");
	data_ptr->equip.spell[2]		= query_result.get_real_value<uint16>("spell2");
	data_ptr->equip.spell[3]		= query_result.get_real_value<uint16>("spell3");

	data_ptr->equip.spell_rate[0]	= query_result.get_real_value<uint16>("spell_rate0");
	data_ptr->equip.spell_rate[1]	= query_result.get_real_value<uint16>("spell_rate1");
	data_ptr->equip.spell_rate[2]	= query_result.get_real_value<uint16>("spell_rate2");

	data_ptr->equip.enchantment		= query_result.get_real_value<uint16>("enchantment");
	data_ptr->equip.creator 		= query_result.get_real_value<uint32>("creator");
}

//低效
void _cache_equip_result_index(ndb_query_result& query_result, nequip_cache* data_ptr)
{
	data_ptr->equip.guid			= query_result.get_real_value<uint64>((uint32)0);
	data_ptr->equip.info_id 		= query_result.get_real_value<uint32>(1);
	data_ptr->equip.count			= query_result.get_real_value<uint16>(2);
	data_ptr->equip.slot			= query_result.get_real_value<uint16>(3);
	data_ptr->equip.bag				= query_result.get_real_value_byte(4);
	data_ptr->equip.flag			= query_result.get_real_value_byte(5);

	data_ptr->equip.quality			= query_result.get_real_value_byte(6);
	data_ptr->equip.level			= query_result.get_real_value_byte(7);
	data_ptr->equip.durability		= query_result.get_real_value<uint32>(8);
	data_ptr->equip.weight			= query_result.get_real_value<uint16>(9);
	data_ptr->equip.sharp			= query_result.get_real_value<uint16>(10);
	data_ptr->equip.solid			= query_result.get_real_value<uint16>(11);
	data_ptr->equip.tenacity		= query_result.get_real_value<uint16>(12);

	data_ptr->equip.stat_type[0]	= query_result.get_real_value_byte(13);
	data_ptr->equip.stat_type[1]	= query_result.get_real_value_byte(14);
	data_ptr->equip.stat_type[2]	= query_result.get_real_value_byte(15);
	data_ptr->equip.stat_type[3]	= query_result.get_real_value_byte(16);
	data_ptr->equip.stat_type[4]	= query_result.get_real_value_byte(17);
	data_ptr->equip.stat_type[5]	= query_result.get_real_value_byte(18);
	data_ptr->equip.stat_type[6]	= query_result.get_real_value_byte(19);
	data_ptr->equip.stat_type[7]	= query_result.get_real_value_byte(20);

	data_ptr->equip.stat_value[0]	= query_result.get_real_value<uint16>(21);
	data_ptr->equip.stat_value[1]	= query_result.get_real_value<uint16>(22);
	data_ptr->equip.stat_value[2]	= query_result.get_real_value<uint16>(23);
	data_ptr->equip.stat_value[3]	= query_result.get_real_value<uint16>(24);
	data_ptr->equip.stat_value[4]	= query_result.get_real_value<uint16>(25);
	data_ptr->equip.stat_value[5]	= query_result.get_real_value<uint16>(26);
	data_ptr->equip.stat_value[6]	= query_result.get_real_value<uint16>(27);
	data_ptr->equip.stat_value[7]	= query_result.get_real_value<uint16>(28);

	data_ptr->equip.gem[0]			= query_result.get_real_value<uint16>(29);
	data_ptr->equip.gem[1]			= query_result.get_real_value<uint16>(30);
	data_ptr->equip.gem[2]			= query_result.get_real_value<uint16>(31);
	data_ptr->equip.gem[3]			= query_result.get_real_value<uint16>(32);
	data_ptr->equip.gem[4]			= query_result.get_real_value<uint16>(33);
	data_ptr->equip.gem[5]			= query_result.get_real_value<uint16>(34);

	data_ptr->equip.spell[0]		= query_result.get_real_value<uint16>(35);
	data_ptr->equip.spell[1]		= query_result.get_real_value<uint16>(36);
	data_ptr->equip.spell[2]		= query_result.get_real_value<uint16>(37);
	data_ptr->equip.spell[3]		= query_result.get_real_value<uint16>(38);

	data_ptr->equip.spell_rate[0]	= query_result.get_real_value<uint16>(39);
	data_ptr->equip.spell_rate[1]	= query_result.get_real_value<uint16>(40);
	data_ptr->equip.spell_rate[2]	= query_result.get_real_value<uint16>(41);

	data_ptr->equip.enchantment		= query_result.get_real_value<uint16>(42);
	data_ptr->equip.creator 		= query_result.get_real_value<uint32>(43);
}

//高效
void _cache_equip_result_atoi(ndb_query_result& query_result, nequip_cache* data_ptr)
{
	data_ptr->equip.guid			= query_result.get_uint64((uint32)0);
	data_ptr->equip.info_id 		= query_result.get_uint32(1);
	data_ptr->equip.count			= query_result.get_uint16(2);
	data_ptr->equip.slot			= query_result.get_uint16(3);
	data_ptr->equip.bag				= query_result.get_uint8(4);
	data_ptr->equip.flag			= query_result.get_uint8(5);

	data_ptr->equip.quality			= query_result.get_uint8(6);
	data_ptr->equip.level			= query_result.get_uint8(7);
	data_ptr->equip.durability		= query_result.get_uint32(8);
	data_ptr->equip.weight			= query_result.get_uint16(9);
	data_ptr->equip.sharp			= query_result.get_uint16(10);
	data_ptr->equip.solid			= query_result.get_uint16(11);
	data_ptr->equip.tenacity		= query_result.get_uint16(12);

	data_ptr->equip.stat_type[0]	= query_result.get_uint8(13);
	data_ptr->equip.stat_type[1]	= query_result.get_uint8(14);
	data_ptr->equip.stat_type[2]	= query_result.get_uint8(15);
	data_ptr->equip.stat_type[3]	= query_result.get_uint8(16);
	data_ptr->equip.stat_type[4]	= query_result.get_uint8(17);
	data_ptr->equip.stat_type[5]	= query_result.get_uint8(18);
	data_ptr->equip.stat_type[6]	= query_result.get_uint8(19);
	data_ptr->equip.stat_type[7]	= query_result.get_uint8(20);

	data_ptr->equip.stat_value[0]	= query_result.get_uint16(21);
	data_ptr->equip.stat_value[1]	= query_result.get_uint16(22);
	data_ptr->equip.stat_value[2]	= query_result.get_uint16(23);
	data_ptr->equip.stat_value[3]	= query_result.get_uint16(24);
	data_ptr->equip.stat_value[4]	= query_result.get_uint16(25);
	data_ptr->equip.stat_value[5]	= query_result.get_uint16(26);
	data_ptr->equip.stat_value[6]	= query_result.get_uint16(27);
	data_ptr->equip.stat_value[7]	= query_result.get_uint16(28);

	data_ptr->equip.gem[0]			= query_result.get_uint16(29);
	data_ptr->equip.gem[1]			= query_result.get_uint16(30);
	data_ptr->equip.gem[2]			= query_result.get_uint16(31);
	data_ptr->equip.gem[3]			= query_result.get_uint16(32);
	data_ptr->equip.gem[4]			= query_result.get_uint16(33);
	data_ptr->equip.gem[5]			= query_result.get_uint16(34);

	data_ptr->equip.spell[0]		= query_result.get_uint16(35);
	data_ptr->equip.spell[1]		= query_result.get_uint16(36);
	data_ptr->equip.spell[2]		= query_result.get_uint16(37);
	data_ptr->equip.spell[3]		= query_result.get_uint16(38);

	data_ptr->equip.spell_rate[0]	= query_result.get_uint16(39);
	data_ptr->equip.spell_rate[1]	= query_result.get_uint16(40);
	data_ptr->equip.spell_rate[2]	= query_result.get_uint16(41);

	data_ptr->equip.enchantment		= query_result.get_uint16(42);
	data_ptr->equip.creator 		= query_result.get_uint32(43);
}

//最低效
void _cache_skill_result(ndb_query_result& query_result, nskill_cache* data_ptr)
{
	data_ptr->id	= query_result.get_real_value<uint32>("id");
	data_ptr->cd	= query_result.get_real_value<uint32>("cd");
}

//低效
void _cache_skill_result_index(ndb_query_result& query_result, nskill_cache* data_ptr)
{
	data_ptr->id	= query_result.get_real_value<uint32>((uint32)0);
	data_ptr->cd	= query_result.get_real_value<uint32>(1);
}

//高效
void _cache_skill_result_atoi(ndb_query_result& query_result, nskill_cache* data_ptr)
{
	data_ptr->id	= query_result.get_uint32((uint32)0);
	data_ptr->cd	= query_result.get_uint32(1);
}

} //namespace nexus

#endif _NDB_NSTORE_CENTER_RESULT_H_