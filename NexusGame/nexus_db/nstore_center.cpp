
#include "nutil.h"
#include "nlog_mt.h"

#include "ndb_mysql/ndb_sql_queue.h"
#include "ndb_mysql/ndb_query_stream.h"
#include "ndb_mysql/ndb_query_result.h"
#include "ndb_mysql/ndb_mysql.h"

#include "nstore_center.h"
#include "nstore_center_sql.h"
#include "nstore_center_result.h"

namespace nexus{


//定义游戏服务器全局数据库访问接口
nstore_center	g_nexus_game_db;

nstore_center::nstore_center(void)
{
	m_game_db = new ndb_mysql;
	if(!m_game_db)
	{
		NLOG_ERROR(_T("%s, create ndb_mysql is wrong!"), __TFUNCTION__);
	}
}


nstore_center::~nstore_center(void)
{
	if( m_game_db )
	{
		delete m_game_db;
		m_game_db = NULL;
	}
}


bool nstore_center::init(std::string host, std::string port, std::string name)
{
	if( !m_game_db ) return false;

	if( !m_game_db->initialize(host.c_str(), "root", port.c_str(), NULL, name.c_str()) )
	{
		NLOG_ERROR(_T("%s, nexus_game_db not connect!"), __TFUNCTION__);
		return false;
	}


	return true;
}


void nstore_center::destroy(void)
{
	if(m_game_db)
	{
		m_game_db->shut_down();
	}
}


void nstore_center::cache_character_role(AccountRoleMap& account_role, Uint32Map& role_account)
{
	if( !m_game_db ) return;

	ndb_query_stream query_stream;
	_cache_character_role(query_stream);

	ndb_query_result query_result;
	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
		for( ; query_result.next_row(); )
		{
			uint32 account_id	= query_result.get_real_value<uint32>("account_id");
			uint32 role_id		= query_result.get_real_value<uint32>("role_id");
			account_role[account_id].insert(role_id);
			role_account[role_id] = account_id;
		}
	}
}


bool nstore_center::cache_character(ncharacter_cache*& info, uint64 role_id)
{
	if( !m_game_db ) return false;

	ndb_query_stream query_stream;
	_cache_character(query_stream, role_id);

	ndb_query_result query_result;

#ifdef	NSTORE_CENTER_CACHE_CHARACTER
	float elapse_time = get_time();
#endif

	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
#ifdef	NSTORE_CENTER_CACHE_CHARACTER
		elapse_time = get_time() - elapse_time;
		NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

		query_result.next_row();
		if( query_result.get_row_count() )
		{
			ncharacter_cache* data_ptr = ncharacter_cache::alloc_character_data();
			_cache_character_result_atoi(query_result, data_ptr);
			info = data_ptr;
			return true;
		}
	}
	return false;
}


void nstore_center::update_character(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return;

	//ndb_query_stream query_stream;
	//_update_character(query_stream, data_ptr);
	//m_game_db->excute(query_stream.get_centence());

	char sql_sentence[1024];
	memset(sql_sentence, '\0', sizeof(sql_sentence));
	_update_character_sprintf(sql_sentence, data_ptr);
	m_game_db->excute(sql_sentence);
}

bool nstore_center::insert_character(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return false;

	ndb_query_stream query_stream;
	_insert_character(query_stream, data_ptr);

	return m_game_db->excute(query_stream.get_centence());
}

void nstore_center::delete_character(uint64 role_id)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_delete_character(query_stream, role_id);

	m_game_db->excute(query_stream.get_centence());
}

void nstore_center::cache_item(uint64 owner_id, ItemMap& itemes)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_cache_item(query_stream, owner_id);

	ndb_query_result query_result;

#ifdef	NSTORE_CENTER_CACHE_ITEM
		float elapse_time = get_time();
#endif

	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
#ifdef	NSTORE_CENTER_CACHE_ITEM
		elapse_time = get_time() - elapse_time;
		NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

		for( ; query_result.next_row(); )
		{
			nitem_cache* data_ptr = nitem_cache::alloc_item_data();
			_cache_item_result_atoi(query_result, data_ptr);
			itemes.insert(std::make_pair(data_ptr->item.guid, data_ptr));
		}
	}
}


void nstore_center::update_item(uint64 owner_id, nitem_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return;



	char sql_sentence[1024];
	memset(sql_sentence, '\0', sizeof(sql_sentence));
	_update_item_sprintf(sql_sentence, owner_id, data_ptr);

#ifdef	NSTORE_CENTER_UPDATE_ITEM
	float elapse_time = get_time();
#endif

	m_game_db->excute(sql_sentence);

#ifdef	NSTORE_CENTER_UPDATE_ITEM
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif
}

bool nstore_center::insert_item(uint64 owner_id, nitem_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return false;

	ndb_query_stream query_stream;
	_insert_item(query_stream, owner_id, data_ptr);

	return m_game_db->excute(query_stream.get_centence());
}

void nstore_center::delete_item(uint64 guid)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_delete_item(query_stream, guid);

	m_game_db->excute(query_stream.get_centence());
}


void nstore_center::cache_equip(uint64 owner_id, EquipMap& equips)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_cache_equip(query_stream, owner_id);

	ndb_query_result query_result;

#ifdef	NSTORE_CENTER_CACHE_EQUIP
	float elapse_time = get_time();
#endif

	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
#ifdef	NSTORE_CENTER_CACHE_EQUIP
		elapse_time = get_time() - elapse_time;
		NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

		for( ; query_result.next_row(); )
		{
			nequip_cache* data_ptr = nequip_cache::alloc_equip_data();
			_cache_equip_result_atoi(query_result, data_ptr);
			equips.insert(std::make_pair(data_ptr->equip.guid, data_ptr));
		}
	}
}


void nstore_center::update_equip(uint64 owner_id, nequip_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return;

	//ndb_query_stream query_stream;
	//_update_equip(query_stream, owner_id, data_ptr);
	//m_game_db->excute(query_stream.get_centence());


	char sql_sentence[1024];
	memset(sql_sentence, '\0', sizeof(sql_sentence));

#ifdef	NSTORE_CENTER_UPDATE_EQUIP_SPRINTF
	float elapse_time = get_time();
#endif

	_update_equip_sprintf(sql_sentence, owner_id, data_ptr);

#ifdef	NSTORE_CENTER_UPDATE_EQUIP_SPRINTF
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif


#ifdef	NSTORE_CENTER_UPDATE_EQUIP
	float elapse_time_exe_sql = get_time();
#endif

	m_game_db->excute(sql_sentence);

#ifdef	NSTORE_CENTER_UPDATE_EQUIP
	elapse_time_exe_sql = get_time() - elapse_time_exe_sql;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time_exe_sql);
#endif
}

bool nstore_center::insert_equip(uint64 owner_id, nequip_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return false;

	ndb_query_stream query_stream;
	_insert_equip(query_stream, owner_id, data_ptr);

	return m_game_db->excute(query_stream.get_centence());
}

void nstore_center::delete_equip(uint64 guid)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_delete_equip(query_stream, guid);

	m_game_db->excute(query_stream.get_centence());
}

void nstore_center::cache_skill(uint64 role_id, SkillMap& skills)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_cache_skill(query_stream, role_id);

	ndb_query_result query_result;

#ifdef	NSTORE_CENTER_CACHE_SKILL
	float elapse_time = get_time();
#endif

	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{

#ifdef	NSTORE_CENTER_CACHE_SKILL
		elapse_time = get_time() - elapse_time;
		NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

		for( ; query_result.next_row(); )
		{
			nskill_cache* data_ptr = nskill_cache::alloc_skill_data();
			_cache_skill_result_atoi(query_result, data_ptr);
			skills.insert(std::make_pair(data_ptr->id, data_ptr));
		}
	}
}

void nstore_center::update_skill(uint64 role_id, nskill_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return;

	char sql_sentence[1024];
	memset(sql_sentence, '\0', sizeof(sql_sentence));
	_update_skill_sprintf(sql_sentence, role_id, data_ptr);

#ifdef	NSTORE_CENTER_UPDATE_SKILL
	float elapse_time = get_time();
#endif

	m_game_db->excute(sql_sentence);

#ifdef	NSTORE_CENTER_UPDATE_SKILL
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif
}

bool nstore_center::insert_skill(uint64 role_id, nskill_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) || UNVALID_POINT(m_game_db) ) return false;

	ndb_query_stream query_stream;
	_insert_skill(query_stream, role_id, data_ptr);

	return m_game_db->excute(query_stream.get_centence());
}

void nstore_center::delete_skill(uint64 role_id, uint32 skill_id)
{
	if( UNVALID_POINT(m_game_db) ) return;

	ndb_query_stream query_stream;
	_delete_skill(query_stream, role_id, skill_id);

	m_game_db->excute(query_stream.get_centence());
}

uint32 nstore_center::get_role_max(void)
{
	if( UNVALID_POINT(m_game_db) ) return 0;

	ndb_query_stream query_stream;
	_get_max_role_id(query_stream);

	ndb_query_result query_result;
	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
		if( !query_result.next_row() )
		{
			return 0;
		}

		if( query_result.get_row_count() && query_result.get_col_count() )
		{
			return query_result.get_real_value<uint32>((uint32)0);
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

uint32 nstore_center::get_account_max(void)
{
	if( UNVALID_POINT(m_game_db) ) return 0;

	ndb_query_stream query_stream;
	_get_max_account(query_stream);

	ndb_query_result query_result;
	if( m_game_db->send_query(query_result, query_stream.get_centence()) )
	{
		if( !query_result.next_row() )
		{
			return 0;
		}

		if( query_result.get_row_count() && query_result.get_col_count() )
		{
			return query_result.get_real_value<uint32>((uint32)0);
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

} //namespace nexus