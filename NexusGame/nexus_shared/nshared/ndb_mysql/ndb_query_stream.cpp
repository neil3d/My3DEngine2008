
#include <boost/program_options.hpp>

#include "nlog_mt.h"
#include "ncore.h"

#include "ndb_query_stream.h"

namespace nexus{

boost::object_pool<ndb_query_stream>  G_query_stream_pool;

// 分配一个ndb_query_stream对象
ndb_query_stream* ndb_query_stream::alloc_query_stream(void)
{
	return ( (ndb_query_stream*)(G_query_stream_pool.construct()) );
}


// 回收一个ndb_query_stream对象
void ndb_query_stream::free_query_stream(ndb_query_stream* qurey_stream)
{
	if(qurey_stream) G_query_stream_pool.free(qurey_stream);
}


std::string ndb_query_stream::filter_format(char* format, ...)
{
	const uint32 QUERY_SQL_LEN = 2048;

	char query_sql[QUERY_SQL_LEN];
	memset(query_sql, '\0', QUERY_SQL_LEN);

	int32 res = 0;
	va_list ap;
	va_start(ap, format);
	res = vsnprintf_s( query_sql, QUERY_SQL_LEN, _TRUNCATE, format, ap );
	va_end(ap);

	assert( res>=0 );

	if( res >= 0 ) return std::string(query_sql);

	return std::string("");
}


ndb_query_stream::ndb_query_stream(void)
{
	m_first_append = false;
}

ndb_query_stream::~ndb_query_stream(void)
{
}


// 获取sql语句
const char* ndb_query_stream::get_centence(void)
{
	return m_sql_centence.c_str();	
}


ndb_query_stream& ndb_query_stream::spell_select(const char* table, const char* seclect)
{
	assert(!is_empty_str(table) && !is_empty_str(seclect));

	clear();
	return  (*this) << ("select ") << (seclect) << (" from ") << (table);
}

	
ndb_query_stream& ndb_query_stream::spell_update(const char* table)
{
	assert(!is_empty_str(table));

	clear();
	return (*this) << ("update ") <<  table << " set ";
}


ndb_query_stream& ndb_query_stream::spell_insert(const char* table)
{
	assert(!is_empty_str(table));

	clear();
	return (*this)<< "insert into " << table << " set ";
}


ndb_query_stream& ndb_query_stream::set_replace(const char* table)
{
	assert(!is_empty_str(table));

	clear();
	return (*this) << "replace into " << table << " set ";
}


ndb_query_stream& ndb_query_stream::spell_delete(const char* table)
{
	assert(!is_empty_str(table));

	clear();
	return (*this) << "delete from " << table;
}


ndb_query_stream& ndb_query_stream::set_where()
{
	return (*this) << " where ";
}


ndb_query_stream& ndb_query_stream::spell_proce_start(const char* proce)
{
	assert(!is_empty_str(proce));

	return (*this) << "CALL " << proce << "(";
}
ndb_query_stream& ndb_query_stream::spell_proce_end(void)
{
	return (*this) << ")";
}


void ndb_query_stream::clear(void)
{
	m_first_append = false;
	m_sql_centence.clear();
}


ndb_query_stream& ndb_query_stream::append_nchar(const nchar* info, uint32 len)
{
	assert(info);

	const uint32 LEN_CHANGE_NUM = 512;
	char value_str[LEN_CHANGE_NUM];
	memset(value_str, '\0', LEN_CHANGE_NUM);

	// 字符串转换
	uint32 cur_len = ::WideCharToMultiByte(CP_UTF8, 0, info, -1, NULL, 0, NULL, NULL);
	if(0==cur_len) return *this;

	::WideCharToMultiByte(CP_UTF8, 0, info, cur_len, value_str, len, NULL, NULL);
	(*this) << value_str;

	return *this;
}


ndb_query_stream& ndb_query_stream::append_col_str_value(const char* col, char* value)
{
	assert(!is_empty_str(col) && !is_empty_str(value));

	if( !m_first_append )
	{
		m_first_append = true;
	(*this) << col << "=" << "'" << value <<  "'" ;
	}
	else
	{
		(*this)<< "," << col << "=" << "'" << value <<  "'" ;
	}

	return *this;
}

ndb_query_stream& ndb_query_stream::append_col_str_value(const char* col, wchar_t* value)
{
	assert(!is_empty_str(col) && !is_empty_str(value));

	const uint32 LEN_CHANGE_NUM = 512;
	char value_str[LEN_CHANGE_NUM];
	memset(value_str, '\0', LEN_CHANGE_NUM);

	// 字符串转换
	uint32 cur_len = ::WideCharToMultiByte(CP_UTF8, 0, value, -1, NULL, 0, NULL, NULL);
	if(0==cur_len) return *this;

	::WideCharToMultiByte(CP_UTF8, 0, value, -1, value_str, cur_len, NULL, NULL);


	if( !m_first_append )
	{
		m_first_append = true;
		(*this) << col << "=" << "'" << value_str <<  "'" ;
	}
	else
	{
		(*this)<< "," << col << "=" << "'" << value_str <<  "'" ;
	}

	return *this;
}


/**
 *	设置select返回从指定位置开始的指定数量的结果集
 *	@remark			: 该接口配合spell_select
 *	@param	num		: 指定的数量
 *	@param	offset:	: 指定的开始位置
 */ 
ndb_query_stream& ndb_query_stream::set_limit(int32 offset, int32 num)
{
	(*this) << " limit " << offset << ',' << num;
	return *this;
}


} //namespace nexus