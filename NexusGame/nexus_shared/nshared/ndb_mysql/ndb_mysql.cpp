
#include <boost/program_options.hpp>


#include "nlog_mt.h"
#include "ncore.h"

#include "ndb_query_stream.h"
#include "ndb_sql_queue.h"
#include "ndb_query_result.h"
#include "ndb_mysql.h"

namespace po = boost::program_options;

namespace nexus{

void output_debug_info(wchar_t* format, ...)
{
#if defined(_DEBUG)
	::OutputDebugStringW(L"\n");

	const uint32 QUERY_SQL_LEN = 1024;
	wchar_t info[QUERY_SQL_LEN];
	wmemset(info, L'\0', QUERY_SQL_LEN);

	int32 res = 0;
	va_list ap;
	va_start(ap, format);
	res = _vsnwprintf_s( info, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);
	info[res+2] = L'\0';
	::OutputDebugStringW(info);

	::OutputDebugStringW(L"\n");
#endif
}


void output_debug_info(char* format, ...)
{
#if defined(_DEBUG)
	::OutputDebugStringA("\n");

	const uint32 QUERY_SQL_LEN = 1024;
	char info[QUERY_SQL_LEN];
	memset(info, '\0', QUERY_SQL_LEN);

	int32 res = 0;
	va_list ap;
	va_start(ap, format);
	res = _vsnprintf_s( info, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);
	info[res+1] = '\0';
	::OutputDebugStringA(info);

	::OutputDebugStringA("\n");
#endif
}


ndb_mysql::ndb_mysql(void)
{
	init();

	m_sql_queue = new ndb_sql_queue();
	assert(NULL != m_sql_queue);

	mysql_library_init(-1, NULL, NULL);
}


ndb_mysql::~ndb_mysql(void)
{
	if( m_sql_queue )
	{
		delete m_sql_queue;
		m_sql_queue = NULL;
	}

	shut_down();

	mysql_library_end();
}


// 远程数据库连接
bool ndb_mysql::initialize(const char* host, const char* user, const char* port, const char* passwd, const char* db_name)
{
	set_cfg(host,user,port,passwd,db_name);

	return _initialize();
}


// 重新连接数据库
bool ndb_mysql::reconnect(void)
{
	shut_down();
	return _initialize();
}


// 关闭数据库
void ndb_mysql::shut_down(void)
{
	if(m_connectiones)
	{
		for(int32 index=0; index<m_connect_num; ++index)
		{
			mysql_close(m_connectiones[index].m_mysql);
		}
		delete [] m_connectiones;
		m_connectiones = NULL;
	}
}


/**
*	向远程数据库发送sql语句
*	@param query	:	sql操作语句
*	@remark	query	:	query不可以为二进制查询语句，只为以'\0'结尾的字符串
*/
bool ndb_mysql::send_query(ndb_query_result& query_result, const char* query)
{
	bool result = false;
	// 任意获得一条连接
	ndb_connection* con = get_free_connection();
	if( con && _excute_query(con->m_mysql, query) )
	{
		MYSQL_RES*	mysql_res	= mysql_store_result(con->m_mysql);
		uint32		rows		= static_cast<uint32>( mysql_affected_rows(con->m_mysql) );
		uint32		cols		= static_cast<uint32>( mysql_field_count(con->m_mysql) );

		query_result.connect(mysql_res, rows, cols);

		if( mysql_more_results(con->m_mysql) )
		{//query为存储过程，返回结果集为多行，去消后续的存储过程状态集
			for( ; !mysql_next_result(con->m_mysql); )
			{
				MYSQL_RES* other_res = mysql_store_result(con->m_mysql);
				mysql_free_result(other_res);
			}
		}
		result = true;
	}
	return_connection(con);

	return result;
	}


/**
*	数据库操作相关
*	@param opration : sql相关操作语句(包括insert, delete, update, replace)
*	@remark			: 和send_query最大的区别是执行sql不需要返回结果集
*/
bool ndb_mysql::excute(const char* query)
	{
	bool result = false;

	// 任意获得一条连接
	ndb_connection* con = get_free_connection();
	if( con )
{
		result = _excute_query(con->m_mysql, query);
}
	return_connection(con);

	return result;
}


/**
 *	转换一个blob字符串
 *	@param out_len : 返回转换后的out_blob的长度
 *	@remark out_len : 需要传入out_len的值 out_len >= (2*len + 1)
 */
bool ndb_mysql::spell_blob_str(const char* blob, uint32 len, char* out_blob, uint32& out_len)
{
	if( !len || !out_blob || out_len<(len*2+1) ) return false;

	ndb_connection* con = get_free_connection();
	if(!con) return false;

	memset( out_blob, '0', len*2+1);	
	out_len = mysql_real_escape_string(con->m_mysql, out_blob, blob, len);
	out_blob[out_len] = '\0';

	return_connection(con);
	return true;
}


// 得到一个空闲连接
ndb_connection* ndb_mysql::get_free_connection(void)
	{
	for(int32 index=0;  ; ++index)
{
		int	cur_index = (index) % m_connect_num;

		ndb_connection* con = m_connectiones + cur_index;
		if( !con )
		{
			_error_prompt("没有任何mysql连接，请检查程序，数据库是否有建立连接");
			return NULL;
		}

		if( con->m_mutex.try_lock() )
			return con;

		if( (m_connect_num-1)== cur_index )
			::Sleep(20);
}

	return NULL;
}


// 使用完后释放一个连接
void ndb_mysql::return_connection(ndb_connection* con)
{
	if( con ) con->m_mutex.unlock();
}


void ndb_mysql::set_cfg(const char* host, const char* user, const char* port, const char* passwd, const char* db_name)
{
		//配置参数
	if( NULL != host )		m_host		= std::string(host);
	if( NULL != user )		m_user		= std::string(user);
	if( NULL != port )		m_port		= boost::lexical_cast<uint32>(std::string(port));
	if( NULL != db_name )	m_db_name	= std::string(db_name);
	if( NULL != passwd )	m_passwd	= std::string(passwd);
}


// 远程数据库连接
bool ndb_mysql::_initialize(void)
{
	if(m_connect_num<1 ) return false;

	m_connectiones = new ndb_connection[m_connect_num];

	for(int32 index=0; index<m_connect_num; ++index)
	{
	MYSQL* mysql_ptr = mysql_init(NULL);
	if(!mysql_ptr)
	{
		_error_sql_prompt(mysql_ptr, "初始化mysql库失败：");
			return false;
	}

	//启动的mysql_client采用的字符集
	mysql_options(mysql_ptr, MYSQL_SET_CHARSET_NAME, m_client_character_set.c_str());

		// 设置mysql断开后，在执行query之前自动重连接
		bool is_reconnect = true;
		mysql_options(mysql_ptr, MYSQL_OPT_RECONNECT, &is_reconnect);

	mysql_ptr = mysql_real_connect( mysql_ptr, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db_name.c_str(), m_port, NULL, m_client_flag);
	if( mysql_ptr )
	{
		if( mysql_autocommit(mysql_ptr, 1) )
		{
			NLOG_ERROR(_T("autocommit not set to 1"));
		}

		mysql_query(mysql_ptr, m_connect_character_sql.c_str());
			m_connectiones[index].m_mysql = mysql_ptr;
		
			NLOG_INFO(_T("success connect to %d database"), m_db_name.c_str());
	}
	else
	{
			_error_sql_prompt(mysql_ptr, "unsuccess to connect to mysql: ");
	mysql_close(mysql_ptr);
			return false;
}
	}
	return true;
}


// 发送sql语句前的基本判断
bool ndb_mysql::pre_send_query(MYSQL* mysql_ptr, const char* query)
{
	if( (NULL==query) || ('\0'==query[0]) )
	{
		_error_prompt("指定的sql query语句为空");
		return false;
	}

	if(!mysql_ptr)
	{
		_error_prompt("未连接远端数据库服务器");
		return false;
	}

	//if( !_is_connect(mysql_ptr) )
	//{//当频繁检测时会降低服务器的效率，去掉
	//	_error_prompt("断开与远端数据库的连接");
	//	return false;
	//}

	return true;
}


// 执行sql语句
bool ndb_mysql::_excute_query(MYSQL* mysql_ptr, const char* query)
{
	if( !pre_send_query(mysql_ptr, query) ) return false;

	uint32	result = mysql_query(mysql_ptr, query);
	if( result )
	{//TODO(LeoChen): 关于发送查询语句的错误处理
		//CR_COMMANDS_OUT_OF_SYNC:	以不恰当的顺序执行了命令
		//CR_SERVER_GONE_ERROR:		MySQL服务器不可用
		//CR_SERVER_LOST:			在查询过程中，与服务器的连接丢失
		//CR_UNKNOWN_ERROR:			出现未知错误

		_error_sql_prompt(mysql_ptr, "sql语句查询执行错误： ");
		if(CR_SERVER_LOST == result)
		{//再次执行一次连接，并发送sql语句
			_is_connect(mysql_ptr);
			result = mysql_query(mysql_ptr, query);
		}
	}

	return(0 == result);
}

bool ndb_mysql::_is_connect(MYSQL* mysql_ptr)
{
	if( !mysql_ptr) return false;

	output_debug_info( "%s", mysql_get_client_info() );
	output_debug_info( "%s", mysql_get_server_info(mysql_ptr) );

	return (0 == mysql_ping(mysql_ptr));
}


void ndb_mysql::init(void)
{
	m_connect_num	= get_cpu_num() * 2 + 2;
	m_connectiones	= NULL;

	m_port			= 3306;
	m_client_flag	= CLIENT_MULTI_RESULTS;//允许返回多结果集,这个主要针对存储过程

	m_client_character_set	= std::string("utf8");
	m_connect_character_sql	= std::string("SET NAMES utf8;");
}


void ndb_mysql::_error_sql_prompt(MYSQL* mysql_ptr, char* error_info)
{
	if(!mysql_ptr) return;

	char* default_error_pre = "mysql wrong info: ";
	if(!error_info)
	{
		error_info = default_error_pre;
	}

	std::string error_msg = mysql_error(mysql_ptr);
	NLOG_ERROR(_T("%s %s"), error_info, error_msg.c_str());
}


void ndb_mysql::_error_prompt(char* error_info)
{
	if(!error_info) return;

	NLOG_ERROR(_T("%s"), error_info);
}


//获取当前pc的CPU数量
uint32 ndb_mysql::get_cpu_num(void)
{
	SYSTEM_INFO sys_info;
	::GetSystemInfo(&sys_info);
	return static_cast<uint32>(sys_info.dwNumberOfProcessors);
}

} //namespace nexus