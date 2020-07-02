#include "nlog_mt.h"
#include "nlan_session.h"
#include "ncache_utility.h"
#include "ncache_center.h"
#include "nstore_center.h"



namespace nexus{


void nlan_session::handle_load_player_data(const nmsg_base* packet)
{
	const tagMsg_W2D_LoadPlayerData* data_ptr = reinterpret_cast<const tagMsg_W2D_LoadPlayerData*>(packet);
	if( !data_ptr) return;

	g_cache_center.m_role_mgr.role_online(data_ptr->player_id, this);
	nrole_info* role_ptr = g_cache_center.m_role_mgr.get_role_info(data_ptr->player_id);
	if(!role_ptr)
	{
		NLOG_ERROR(_T("player %u is not exist!"), data_ptr->player_id);
		return;
	}

	//发送角色相关信息
	role_ptr->send_load_msg();

	////发送角色基本信息
	//ncharacter_cache* cache_ptr = role_ptr->get_character().get_data();
	//tagMsg_D2W_LoadPlayerRoleInfo role_msg;
	//role_msg.account_id	= cache_ptr->account_id;
	//role_msg.role_id	= cache_ptr->role_id;
	//memcpy(role_msg.name, cache_ptr->name, sizeof(cache_ptr->name));
	//role_msg.sex		= cache_ptr->sex;
	//role_msg.race		= cache_ptr->race;
	//role_msg.class_type	= cache_ptr->class_type;
	//role_msg.map_id		= cache_ptr->map_id;
	//role_msg.last_time	= cache_ptr->logout_time;
	//send_message( &role_msg, sizeof(role_msg) );

	////发送技能信息
	//tagMsg_D2W_LoadPlayerSkill skill_msg;
	//SkillMap& skilles	= role_ptr->get_skilles().get_skilles();
	//skill_msg.role_id	= data_ptr->player_id;
	//skill_msg.num		= 0;
	//for( SkillMapIt it=skilles.begin(); skilles.end()!=it; ++it )
	//{
	//	skill_msg.skilles[skill_msg.num++] = it->first;
	//}
	//send_message( &skill_msg, sizeof(skill_msg) );

	// //todo : 发送物品信息
	// //todo : 发送装备信息
	// //todo : 发送其他相关信息
}


																						
void nlan_session::handle_unload_player_data(const nmsg_base* packet)
{
	const tagMsg_W2D_UnloadPlayer* data_ptr = reinterpret_cast<const tagMsg_W2D_UnloadPlayer*>(packet);
	if( !data_ptr) return;

	if( !g_cache_center.m_role_mgr.is_online(data_ptr->role_id) )
	{
		NLOG_ERROR(_T("player %u is not exist!"), data_ptr->role_id);
		return;
	}

	ncharacter_cache* cache_ptr = g_cache_center.m_role_mgr.get_character_cache(data_ptr->role_id);
	if(!cache_ptr) return;

	//立即保存角色基本信息
	cache_ptr->logout_time;
	cache_ptr->change = true;
	g_nexus_game_db.update_character(cache_ptr);

	//角色下线保存其他信息
	g_cache_center.m_role_mgr.role_offline(data_ptr->role_id);
}

void nlan_session::handle_create_character(const nmsg_base* packet)
{
	const tagMsg_W2D_CharCreate* data_ptr = reinterpret_cast<const tagMsg_W2D_CharCreate*>(packet);
	if( !data_ptr ) return;

	tagMsg_D2W_CharCreate msg;

	ncharacter_cache* character_ptr = ncharacter_cache::alloc_character_data();
	character_ptr->account_id	= data_ptr->account_id;
	memcpy(character_ptr->name, data_ptr->name, sizeof(data_ptr->name));
	character_ptr->sex			= data_ptr->sex;
	character_ptr->race			= data_ptr->race;
	character_ptr->class_type	= data_ptr->class_type;
	character_ptr->role_id		= 1 + g_nexus_game_db.get_role_max();
	character_ptr->change		= false;

	g_cache_center.insert_character(character_ptr);

	msg.error		= EDBError_CreateCharacterSucess;
	msg.player_id	= character_ptr->role_id;
	send_message(&msg, sizeof(msg));
}

void nlan_session::handle_delete_character(const nmsg_base* packet)
{
	const tagMsg_W2D_CharDelete* data_ptr = reinterpret_cast<const tagMsg_W2D_CharDelete*>(packet);
	if( !data_ptr ) return;

	tagMsg_D2W_CharDelete msg;
	msg.client_id = data_ptr->client_id;

	ncharacter_cache* character_ptr = g_cache_center.m_role_mgr.get_character_cache(data_ptr->player_id);
	if( !character_ptr )
	{
		msg.error = EDBError_NoExistCharacter;
		send_message(&msg, sizeof(msg));
		return;
	}

	g_cache_center.destroy_character(data_ptr->player_id);
	msg.error = EDBError_DeleteCharacterSucess;
	send_message(&msg, sizeof(msg));
}

void nlan_session::handle_operate_skill(const nmsg_base* packet)
{
	const tagMsg_W2D_Operate_Skill* data_ptr = reinterpret_cast<const tagMsg_W2D_Operate_Skill*>(packet);
	if( !data_ptr ) return;

	tagMsg_D2W_Operate_Skill msg;
	msg.role_id = data_ptr->role_id;
	msg.op_type	= data_ptr->op_type;
	msg.result	= EDBError_OperateSkillSuccess;

	ncharacter_cache* character_ptr = g_cache_center.m_role_mgr.get_character_cache(data_ptr->role_id);
	if( !character_ptr )
	{
		msg.result = EDBError_NoExistCharacter;
		send_message(&msg, sizeof(msg));
		return;
	}

	nrole_info* role_ptr = g_cache_center.m_role_mgr.get_role_info(data_ptr->role_id);
	if( !character_ptr )
	{
		msg.result = EDBError_NoRoleOffLine;
		send_message(&msg, sizeof(msg));
		return;
	}

	nskill&	skilles = role_ptr->get_skilles();
	switch(data_ptr->op_type)
	{
	case EOperateSkill_Add:
		{
			nskill_cache* cache_ptr = nskill_cache::alloc_skill_data();
			cache_ptr->id		= data_ptr->skill_id;
			cache_ptr->cd		= 0;
			cache_ptr->change	= false;
			skilles.insert(cache_ptr);
		}
		break;
	case EOperateSkill_Delete:
		{
			skilles.destroy(data_ptr->skill_id);
		}
		break;
	}

	send_message(&msg, sizeof(msg));
}

} //namespace nexus