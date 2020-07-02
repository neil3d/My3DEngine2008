/**
 *	nexus db - nstore_center_sql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NSTORE_CENTER_SQL_H_
#define _NDB_NSTORE_CENTER_SQL_H_

#pragma warning(disable : 4996)

namespace nexus{

void _get_max_role_id(ndb_query_stream& query_stream)
{
	query_stream.spell_proce_start("get_role_max");
	query_stream.spell_proce_end();
}

void _get_max_account(ndb_query_stream& query_stream)
{
	query_stream.spell_proce_start("get_account_max");
	query_stream.spell_proce_end();
}


void _cache_character_role(ndb_query_stream& query_stream)
{
	query_stream.spell_proce_start("character_role");
	query_stream.spell_proce_end();
}


void _cache_character(ndb_query_stream& query_stream, uint64 role_id)
{
	query_stream.spell_proce_start("character_cache");
	query_stream << role_id;
	query_stream.spell_proce_end();
}


void _update_character(ndb_query_stream& query_stream, ncharacter_cache* data_ptr)
{
	query_stream.spell_proce_start("character_update");
	query_stream << data_ptr->role_id << ',';

	query_stream << ( (uint32)(data_ptr->level) ) << ',';
	query_stream << data_ptr->cur_exp << ',';
	query_stream << data_ptr->hp << ',';
	query_stream << data_ptr->mp << ',';

	query_stream << data_ptr->map_id << ',';
	query_stream << data_ptr->x << ',';
	query_stream << data_ptr->y << ',';
	query_stream << data_ptr->z << ',';
	query_stream << data_ptr->orient << ',';

	query_stream << data_ptr->login_time << ',';
	query_stream << data_ptr->login_time << ',';
	query_stream << data_ptr->online_time;

	query_stream.spell_proce_end();
}

void _update_character_sprintf(char* sql_sentence, ncharacter_cache* data_ptr)
{
	if(!sql_sentence) return;
	sprintf(sql_sentence, "CALL %s(\
%llu,\
%d,%d,%d,%d,\
%d,%f,%f,%f,%f,\
%d,%d,%d)","character_update",\
/*	1	*/	data_ptr->role_id,\
/*	2	*/	data_ptr->level,data_ptr->cur_exp,data_ptr->hp,data_ptr->mp,\
/*	6	*/	data_ptr->map_id,data_ptr->x,data_ptr->y,data_ptr->z,data_ptr->orient,\
/*	11	*/	data_ptr->login_time,data_ptr->login_time,data_ptr->online_time);

}

void _insert_character(ndb_query_stream& query_stream, ncharacter_cache* data_ptr)
{
	query_stream.spell_proce_start("character_insert");
	query_stream << data_ptr->account_id << ',';
	query_stream << data_ptr->role_id << ',';

	query_stream.append_nchar(data_ptr->name, MAX_ACCOUNT_LEN) << ',';
	query_stream << ( (uint32)(data_ptr->sex) ) << ',';
	query_stream << ( (uint32)(data_ptr->race) ) << ',';
	query_stream << ( (uint32)(data_ptr->class_type) ) << ',';
	query_stream << ( (uint32)(data_ptr->level) ) << ',';
	query_stream << data_ptr->cur_exp << ',';
	query_stream << data_ptr->hp << ',';
	query_stream << data_ptr->mp << ',';

	query_stream << data_ptr->map_id << ',';
	query_stream << data_ptr->x << ',';
	query_stream << data_ptr->y << ',';
	query_stream << data_ptr->z << ',';
	query_stream << data_ptr->orient << ',';

	query_stream << data_ptr->create_time << ',';
	query_stream << data_ptr->login_time << ',';
	query_stream << data_ptr->login_time << ',';
	query_stream << data_ptr->online_time;

	query_stream.spell_proce_end();
}

void _insert_character_sprintf(char* sql_sentence, ncharacter_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(\
%d,%llu,\
%s,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%f,%f,%f,%f,\
%d,%d,%d,%d)","character_insert",\
		/*	1	*/	data_ptr->account_id, data_ptr->role_id,
		/*	3	*/	data_ptr->name, data_ptr->sex, data_ptr->race, data_ptr->class_type,
		/*	7	*/	data_ptr->level, data_ptr->cur_exp, data_ptr->hp, data_ptr->mp,
		/*	11	*/	data_ptr->map_id, data_ptr->x, data_ptr->y, data_ptr->z, data_ptr->orient,
		/*	16	*/	data_ptr->create_time, data_ptr->login_time, data_ptr->login_time, data_ptr->online_time);
}

void _delete_character(ndb_query_stream& query_stream, uint64 role_id)
{
	query_stream.spell_proce_start("character_delete");
	query_stream << role_id;
	query_stream.spell_proce_end();
}

void _cache_item(ndb_query_stream& query_stream, uint64 owner_id)
{
	query_stream.spell_proce_start("item_cache");
	query_stream<<owner_id;
	query_stream.spell_proce_end();
}


void _update_item(ndb_query_stream& query_stream, uint64 owner_id, nitem_cache* data_ptr)
{
	query_stream.spell_proce_start("item_update");
	query_stream << data_ptr->item.guid << ',';
	query_stream << owner_id << ',';
	query_stream << data_ptr->item.info_id << ',';
	query_stream << data_ptr->item.count << ',';
	query_stream << data_ptr->item.slot << ',';
	query_stream << ((uint32)(data_ptr->item.bag)) << ',';
	query_stream << ((uint32)(data_ptr->item.flag));
	query_stream.spell_proce_end();
}


void _update_item_sprintf(char* sql_sentence, uint64 owner_id, nitem_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(\
%llu,%llu,\
%d,%d,\
%d,%d,%d)", "item_update",\
/*	1	*/	data_ptr->item.guid, owner_id ,\
/*	3	*/	data_ptr->item.info_id,data_ptr->item.count,\
/*	5	*/	data_ptr->item.slot,data_ptr->item.bag,data_ptr->item.flag);
}


void _insert_item(ndb_query_stream& query_stream, uint64 owner_id, nitem_cache* data_ptr)
{
	query_stream.spell_proce_start("item_insert");
	query_stream << data_ptr->item.guid << ',';
	query_stream << owner_id << ',';
	query_stream << data_ptr->item.info_id << ',';

	query_stream << data_ptr->item.count << ',';
	query_stream << data_ptr->item.slot << ',';
	query_stream << ((uint32)(data_ptr->item.bag)) << ',';
	query_stream << ((uint32)(data_ptr->item.flag));

	query_stream.spell_proce_end();
}


void _insert_item_sprintf(char* sql_sentence, uint64 owner_id, nitem_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(\
%llu,%llu,\
%d,%d,\
%d,%d,%d)", "item_insert",\
/*	1	*/	data_ptr->item.guid, owner_id ,\
/*	3	*/	data_ptr->item.info_id,data_ptr->item.count,\
/*	5	*/	data_ptr->item.slot,data_ptr->item.bag,data_ptr->item.flag);
}

void _delete_item(ndb_query_stream& query_stream, uint64 guid)
{
	query_stream.spell_proce_start("item_delete");
	query_stream << guid;
	query_stream.spell_proce_end();
}


void _cache_equip(ndb_query_stream& query_stream, uint64 owner_id)
{
	query_stream.spell_proce_start("equip_cache");
	query_stream << owner_id;
	query_stream.spell_proce_end();
}


void _update_equip(ndb_query_stream& query_stream, uint64 owner_id, nequip_cache* data_ptr)
{
	query_stream.spell_proce_start("equip_update");

	query_stream << data_ptr->equip.guid << ',';
	query_stream << owner_id << ',';
	query_stream << data_ptr->equip.info_id << ',';
	query_stream << data_ptr->equip.count << ',';
	query_stream << data_ptr->equip.slot << ',';
	query_stream << ((uint32)(data_ptr->equip.bag)) << ',';
	query_stream << ((uint32)(data_ptr->equip.flag)) << ',';

	query_stream << ((uint32)(data_ptr->equip.quality)) << ',';
	query_stream << ((uint32)(data_ptr->equip.level)) << ',';
	query_stream << data_ptr->equip.durability << ',';
	query_stream << data_ptr->equip.weight << ',';
	query_stream << data_ptr->equip.sharp << ',';
	query_stream << data_ptr->equip.solid << ',';
	query_stream << data_ptr->equip.tenacity << ',';

	query_stream << ((uint32)(data_ptr->equip.stat_type[0])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[1])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[2])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[3])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[4])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[5])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[6])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[7])) << ',';

	query_stream << data_ptr->equip.stat_value[0] << ',';
	query_stream << data_ptr->equip.stat_value[1] << ',';
	query_stream << data_ptr->equip.stat_value[2] << ',';
	query_stream << data_ptr->equip.stat_value[3] << ',';
	query_stream << data_ptr->equip.stat_value[4] << ',';
	query_stream << data_ptr->equip.stat_value[5] << ',';
	query_stream << data_ptr->equip.stat_value[6] << ',';
	query_stream << data_ptr->equip.stat_value[7] << ',';


	query_stream << data_ptr->equip.gem[0] << ',';
	query_stream << data_ptr->equip.gem[1] << ',';
	query_stream << data_ptr->equip.gem[2] << ',';
	query_stream << data_ptr->equip.gem[3] << ',';
	query_stream << data_ptr->equip.gem[4] << ',';
	query_stream << data_ptr->equip.gem[5] << ',';

	query_stream << data_ptr->equip.spell[0] << ',';
	query_stream << data_ptr->equip.spell[1] << ',';
	query_stream << data_ptr->equip.spell[2] << ',';
	query_stream << data_ptr->equip.spell[3] << ',';

	query_stream << data_ptr->equip.spell_rate[0] << ',';
	query_stream << data_ptr->equip.spell_rate[1] << ',';
	query_stream << data_ptr->equip.spell_rate[2] << ',';

	query_stream << data_ptr->equip.enchantment << ',';
	query_stream << data_ptr->equip.creator;

	query_stream.spell_proce_end();
}

void _update_equip_sprintf(char* sql_sentence, uint64 owner_id, nequip_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(\
%llu,%llu,\
%d,%d,\
%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,\
%d,%d\
)", "equip_update",\
/*	1	*/	data_ptr->equip.guid ,owner_id,\
/*	3	*/	data_ptr->equip.info_id,data_ptr->equip.count,\
/*	5	*/	data_ptr->equip.slot,data_ptr->equip.bag,data_ptr->equip.flag,\
/*	8	*/	data_ptr->equip.quality,data_ptr->equip.level,data_ptr->equip.durability,\
/*	11	*/	data_ptr->equip.weight,data_ptr->equip.sharp,data_ptr->equip.solid,data_ptr->equip.tenacity,\
/*	15	*/	data_ptr->equip.stat_type[0],data_ptr->equip.stat_type[1],data_ptr->equip.stat_type[2],data_ptr->equip.stat_type[3],\
/*	19	*/	data_ptr->equip.stat_type[4],data_ptr->equip.stat_type[5],data_ptr->equip.stat_type[6],data_ptr->equip.stat_type[7],\
/*	23	*/	data_ptr->equip.stat_value[0],data_ptr->equip.stat_value[1],data_ptr->equip.stat_value[2],data_ptr->equip.stat_value[3],\
/*	27	*/	data_ptr->equip.stat_value[4],data_ptr->equip.stat_value[5],data_ptr->equip.stat_value[6],data_ptr->equip.stat_value[7],\
/*	31	*/	data_ptr->equip.gem[0],data_ptr->equip.gem[1],data_ptr->equip.gem[2],\
/*	34	*/	data_ptr->equip.gem[3],data_ptr->equip.gem[4],data_ptr->equip.gem[5],\
/*	37	*/	data_ptr->equip.spell[0],data_ptr->equip.spell[1],data_ptr->equip.spell[2],data_ptr->equip.spell[3],\
/*	41	*/	data_ptr->equip.spell_rate[0],data_ptr->equip.spell_rate[1],data_ptr->equip.spell_rate[2],\
/*	44	*/	data_ptr->equip.enchantment ,data_ptr->equip.creator);
}


void _insert_equip(ndb_query_stream& query_stream, uint64 owner_id, nequip_cache* data_ptr)
{
	query_stream.spell_proce_start("equip_insert");

	query_stream << data_ptr->equip.guid << ',';
	query_stream << owner_id << ',';
	query_stream << data_ptr->equip.info_id << ',';
	query_stream << data_ptr->equip.count << ',';
	query_stream << data_ptr->equip.slot << ',';
	query_stream << ((uint32)(data_ptr->equip.bag)) << ',';
	query_stream << ((uint32)(data_ptr->equip.flag)) << ',';

	query_stream << ((uint32)(data_ptr->equip.quality)) << ',';
	query_stream << ((uint32)(data_ptr->equip.level)) << ',';
	query_stream << data_ptr->equip.durability << ',';
	query_stream << data_ptr->equip.weight << ',';
	query_stream << data_ptr->equip.sharp << ',';
	query_stream << data_ptr->equip.solid << ',';
	query_stream << data_ptr->equip.tenacity << ',';

	query_stream << ((uint32)(data_ptr->equip.stat_type[0])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[1])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[2])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[3])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[4])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[5])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[6])) << ',';
	query_stream << ((uint32)(data_ptr->equip.stat_type[7])) << ',';

	query_stream << data_ptr->equip.stat_value[0] << ',';
	query_stream << data_ptr->equip.stat_value[1] << ',';
	query_stream << data_ptr->equip.stat_value[2] << ',';
	query_stream << data_ptr->equip.stat_value[3] << ',';
	query_stream << data_ptr->equip.stat_value[4] << ',';
	query_stream << data_ptr->equip.stat_value[5] << ',';
	query_stream << data_ptr->equip.stat_value[6] << ',';
	query_stream << data_ptr->equip.stat_value[7] << ',';


	query_stream << data_ptr->equip.gem[0] << ',';
	query_stream << data_ptr->equip.gem[1] << ',';
	query_stream << data_ptr->equip.gem[2] << ',';
	query_stream << data_ptr->equip.gem[3] << ',';
	query_stream << data_ptr->equip.gem[4] << ',';
	query_stream << data_ptr->equip.gem[5] << ',';

	query_stream << data_ptr->equip.spell[0] << ',';
	query_stream << data_ptr->equip.spell[1] << ',';
	query_stream << data_ptr->equip.spell[2] << ',';
	query_stream << data_ptr->equip.spell[3] << ',';

	query_stream << data_ptr->equip.spell_rate[0] << ',';
	query_stream << data_ptr->equip.spell_rate[1] << ',';
	query_stream << data_ptr->equip.spell_rate[2] << ',';

	query_stream << data_ptr->equip.enchantment << ',';
	query_stream << data_ptr->equip.creator;

	query_stream.spell_proce_end();
}


void _insert_equip_sprintf(char* sql_sentence, uint64 owner_id, nequip_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(\
%llu,%llu,\
%d,%d,\
%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,\
%d,%d\
)", "equip_insert",\
/*	1	*/	data_ptr->equip.guid ,owner_id,\
/*	3	*/	data_ptr->equip.info_id,data_ptr->equip.count,\
/*	5	*/	data_ptr->equip.slot,data_ptr->equip.bag,data_ptr->equip.flag,\
/*	8	*/	data_ptr->equip.quality,data_ptr->equip.level,data_ptr->equip.durability,\
/*	11	*/	data_ptr->equip.weight,data_ptr->equip.sharp,data_ptr->equip.solid,data_ptr->equip.tenacity,\
/*	15	*/	data_ptr->equip.stat_type[0],data_ptr->equip.stat_type[1],data_ptr->equip.stat_type[2],data_ptr->equip.stat_type[3],\
/*	19	*/	data_ptr->equip.stat_type[4],data_ptr->equip.stat_type[5],data_ptr->equip.stat_type[6],data_ptr->equip.stat_type[7],\
/*	23	*/	data_ptr->equip.stat_value[0],data_ptr->equip.stat_value[1],data_ptr->equip.stat_value[2],data_ptr->equip.stat_value[3],\
/*	27	*/	data_ptr->equip.stat_value[4],data_ptr->equip.stat_value[5],data_ptr->equip.stat_value[6],data_ptr->equip.stat_value[7],\
/*	31	*/	data_ptr->equip.gem[0],data_ptr->equip.gem[1],data_ptr->equip.gem[2],\
/*	34	*/	data_ptr->equip.gem[3],data_ptr->equip.gem[4],data_ptr->equip.gem[5],\
/*	37	*/	data_ptr->equip.spell[0],data_ptr->equip.spell[1],data_ptr->equip.spell[2],data_ptr->equip.spell[3],\
/*	41	*/	data_ptr->equip.spell_rate[0],data_ptr->equip.spell_rate[1],data_ptr->equip.spell_rate[2],\
/*	44	*/	data_ptr->equip.enchantment ,data_ptr->equip.creator);
}

void _delete_equip(ndb_query_stream& query_stream, uint64 guid)
{
	query_stream.spell_proce_start("equip_delete");
	query_stream << guid;
	query_stream.spell_proce_end();
}

void _cache_skill(ndb_query_stream& query_stream, uint64 role_id)
{
	query_stream.spell_proce_start("skill_cache");
	query_stream << role_id;
	query_stream.spell_proce_end();
}

void _update_skill(ndb_query_stream& query_stream, uint64 role_id, nskill_cache* data_ptr)
{
	query_stream.spell_proce_start("skill_update");
	query_stream << role_id << ',';
	query_stream << data_ptr->id << ',';
	query_stream << data_ptr->cd;
	query_stream.spell_proce_end();
}

void _update_skill_sprintf(char* sql_sentence, uint64 role_id, nskill_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu,%d,%d)", "skill_update",role_id,data_ptr->id,data_ptr->cd);
}

void _insert_skill(ndb_query_stream& query_stream, uint64 role_id, nskill_cache* data_ptr)
{
	query_stream.spell_proce_start("skill_insert");
	query_stream << role_id << ',';
	query_stream << data_ptr->id << ',';
	query_stream << data_ptr->cd;
	query_stream.spell_proce_end();
}

void _insert_skill_sprintf(char* sql_sentence, uint64 role_id, nskill_cache* data_ptr)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu,%d,%d)", "skill_insert",role_id,data_ptr->id,data_ptr->cd);
}

void _delete_skill(ndb_query_stream& query_stream, uint64 role_id, uint32 skill_id)
{
	query_stream.spell_proce_start("skill_delete");
	query_stream << role_id << ',';
	query_stream << skill_id;
	query_stream.spell_proce_end();
}

} //namespace nexus

#endif _NDB_NSTORE_CENTER_SQL_H_