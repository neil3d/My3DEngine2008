
#include "nutil.h"
#include "nlog_mt.h"
#include "ndb_mysql/ndb_sql_queue.h"
#include "ndb_mysql/ndb_query_stream.h"
#include "ndb_mysql/ndb_query_result.h"
#include "ndb_mysql/ndb_mysql.h"

#include "nplayer_account_mgr.h"
#include "nlogin_player.h"
#include "nlogin_db.h"
#include "nlogin_db_sql.h"
#include "nlogin_db_result.h"

namespace nexus{


//定义登陆服务器全局数据库访问接口
nlogin_db	g_login_db;


nlogin_db::nlogin_db(void)
{
	m_db = new ndb_mysql;
	if(!m_db)
	{
		NLOG_ERROR(_T("%s, create ndb_mysql is wrong!"), __TFUNCTION__);
	}
}


nlogin_db::~nlogin_db(void)
{
	if( m_db )
	{
		delete m_db;
		m_db = NULL;
	}
}


bool nlogin_db::init(std::string host, std::string port, std::string name)
{
	if(!m_db) return false;

	if( !m_db->initialize(host.c_str(), "root", port.c_str(), NULL, name.c_str()) )
	{
		NLOG_ERROR(_T("%s, login_db not connect!"), __TFUNCTION__);
	}

	return true;
}


void nlogin_db::destroy(void)
{
	if(m_db)
	{
		m_db->shut_down();
	}
}


// 缓存指定账号信息
bool nlogin_db::cache_account_info(uint32 account_id, naccount_info* info_ptr)
{
	if(!info_ptr) return false;

#ifdef	NLOGIN_DB_CACHE_ACCOUNT_INFO
	float elapse_time = get_time();
#endif

	char sql_sentence[128];
	_cache_account_info_sprintf(sql_sentence, account_id);

	ndb_query_result query_result;
	if( !m_db->send_query(query_result, sql_sentence) )
	{
		NLOG_ERROR(_T("%s, worong to send_query"), __TFUNCTION__);
		return false;
	}

	if( !query_result.next_row() )
		{
		NLOG_ERROR(_T("%s, not have account %llu"), __TFUNCTION__, account_id);
		return false;
	}

	if( !query_result.get_row_count() || !query_result.get_col_count() )
			{
		NLOG_ERROR(_T("%s, not have account %llu"), __TFUNCTION__, account_id);
		return false;
			}

	_cache_account_result_atoi(query_result, info_ptr);

#ifdef	NLOGIN_DB_CACHE_ACCOUNT_INFO
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, the time is %f"), __TFUNCTION__, elapse_time);
#endif
	return true;
}


// 指定账号是否存在
bool nlogin_db::exist_account(uint32 account_id)
{
#ifdef	NLOGIN_DB_EXIST_ACCOUNT
	float elapse_time = get_time();
#endif

	char sql_sentence[64];
	_exist_account_sprintf(sql_sentence, account_id);

	ndb_query_result query_result;
	if( !m_db->send_query(query_result, sql_sentence) )
	{
		NLOG_ERROR(_T("%s, wrong to send_query"), __TFUNCTION__);
		return false;
	}

#ifdef	NLOGIN_DB_EXIST_ACCOUNT
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, the time is %f"), __TFUNCTION__, elapse_time);
#endif

	if( !query_result.next_row() )
		return false;

	if( query_result.get_row_count() && query_result.get_col_count() )
		return true;

	return false;
}


/**
 *	索取指定账号的密码
 *	@return psd : 从DB中获取的密码
 */
bool nlogin_db::query_psd(uint32 account_id, char* psd)
{
	if( !is_exist_db() ) return false;

	char sql_sentence[512];
	_query_psd_sprintf(sql_sentence, account_id);

	ndb_query_result query_result;
	if( !m_db->send_query(query_result, sql_sentence) )
	{
		NLOG_ERROR(_T("%s, wrong to send_query"), __TFUNCTION__);
		return false;
	}

	if( !query_result.next_row() )
		return false;

	std::string qsd_from_db(query_result.get_col_value((uint32)0));
	memcpy(psd, qsd_from_db.c_str(), qsd_from_db.size());

	return true;
}


// 修正玩家登陆标志
void nlogin_db::fix_player_login_status(uint32 account_id, EPlayerLoginStatus status)
{
	char sql_sentence[512];

#ifdef	NLOGIN_DB_FIX_LOGIN_STATUS
	float elapse_time = get_time();
#endif

	_fix_login_status_sprintf(sql_sentence, account_id, (uint8)status);
	m_db->excute(sql_sentence);

#ifdef	NLOGIN_DB_FIX_LOGIN_STATUS
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, the time is %f"), __TFUNCTION__, elapse_time);
#endif
}


// 获取玩家密保
bool nlogin_db::get_mibao(uint32 account_id, char* mibao, uint32 len)
{
	bool result = false;
	ndb_query_stream query_stream;
	_get_mibao(query_stream, account_id, mibao);

	ndb_query_result query_result;
	if( m_db->send_query( query_result, query_stream.get_centence()) )
	{
		if( 1== query_result.get_row_count() )
		{
			if( query_result.get_real_value_blob((uint32)0, mibao, len) )
				result = true;
		}
	}

	return result;
}


// 获取玩家的privledge
uint8 nlogin_db::query_account_privilege(uint32 account_id)
{
	char sql_sentence[512];
	_query_privilege_sprintf(sql_sentence, account_id);

	ndb_query_result query_result;
	if( m_db->send_query( query_result, sql_sentence) )
	{
		if( !query_result.next_row() )
			return false;

		if( query_result.get_row_count() && query_result.get_col_count() )
			return query_result.get_uint8((uint32)0);
		}

	return 0;
}


// 玩家登出
void nlogin_db::login_out(uint32 account_id, uint32 last_ip)
{
	ndb_query_stream query_stream;
	_login_out(query_stream, account_id, last_ip);
	m_db->excute(query_stream.get_centence());
}


// 获取账号表中账号的数量
uint32 nlogin_db::query_account_num(void)
{
	char* sql_sentence = "CALL account_num()";

	ndb_query_result query_result;
	if( m_db->send_query(query_result, sql_sentence) )
	{
		if( query_result.next_row() )
		{
			if( query_result.get_row_count() && query_result.get_col_count() )
			{
				return query_result.get_uint32((uint32)0);
			}
		}

		return 0;
	}
	return 0;
}


// 增加账号
bool nlogin_db::insert_account(naccount_info* data_ptr)
{
	if( !data_ptr ) return false;

	char sql_sentence[1024];
	memset(sql_sentence, 0, sizeof(sql_sentence));

	_insert_account_sprintf(sql_sentence, data_ptr);
	
	return m_db->excute(sql_sentence);
}


// 修改所有的账号为指定状态
bool nlogin_db::fix_all_login_status(EPlayerLoginStatus status)
{
	char sql_sentence[64];
	memset(sql_sentence, 0, sizeof(sql_sentence));

	_fix_all_login_status_sprintf(sql_sentence, (uint8)status);

	return m_db->excute(sql_sentence);
}


} //namespace nexus