
#include "nlog_mt.h"
#include "ndb_query_result.h"



namespace nexus{

ndb_query_result::ndb_query_result(void)
{
	_init(NULL, 0, 0);
}

ndb_query_result::~ndb_query_result(void)
{
	_un_connect();
}

// 和指定新返回结果集关联
void ndb_query_result::connect(MYSQL_RES* mysql_res, uint32 rows, uint32 fields)
{
	//解除和旧的结果集的关联
	_un_connect();

	_init(mysql_res, rows, fields);
}

bool ndb_query_result::next_row(void)
{
	if( !m_mysql_res)
		return false;

	m_cur_row= mysql_fetch_row(m_mysql_res);
	if( !m_cur_row )
		return false;

	if( !m_rows || !m_fieldes )
		return false;

	if( NULL == m_cur_row[0] )
		return false;

	unsigned long* len_ptr = mysql_fetch_lengths(m_mysql_res);
	if( !len_ptr )
		return false;

	return true;
}

char* ndb_query_result::get_col_name(uint32 col)
{
	if( is_valid_col(col) )
		return m_fieldes[col].name;
	return NULL;
}


char* ndb_query_result::get_col_value(uint32 col)
{
	if( is_valid_col(col) )
		return m_cur_row[col];
	return NULL;
	}

char* ndb_query_result::get_col_value(const char* col_name)
	{
	int32 col = get_col_by_name(col_name);

	if( is_valid_col(col) )	return m_cur_row[col];

	return NULL;
	}

char* ndb_query_result::operator[](uint32 col) const
	{
	if( (col>=0) && (col<m_cols) && (m_cur_row!=NULL) )
		return m_cur_row[col];

	return NULL;
	}

char* ndb_query_result::operator[](const char* col_name) const
{
	if(col_name)
	{
	for( uint32 col=0; col<m_cols; ++col )
	{
			if( 0== memcmp(col_name, m_fieldes[col].name, strlen(col_name)) )
			{
				return m_cur_row[col];
	}
}
	}
	return NULL;
}

/**
 *	获取指定列的字符信息
 *	@return : 实际的长度(-1:无效返回)
 *	@param out value : 输出结果
 *	@param len : 输出目标的长度
 */
int32 ndb_query_result::get_real_value_str(uint32 col, nchar* value, uint32 len)
{
	if( !is_type(col, EFType_STRING) )
		return -1;

	uint32 cur_len = ::MultiByteToWideChar(CP_UTF8, 0, m_cur_row[col], -1, NULL, 0);
	if( cur_len >= len )
		return -1;

	::MultiByteToWideChar(CP_UTF8, 0, m_cur_row[col], -1, value, len);
	return cur_len;
}


int32 ndb_query_result::get_real_value_str(const char* col_name, nchar* value, uint32 len)
{
	int32 col = get_col_by_name(col_name);
	return get_real_value_str(col, value, len);
}

bool ndb_query_result::get_real_value_blob(uint32 col, char* value, uint32 len)
{
	if(!m_mysql_res || !m_cur_row || !m_fieldes)
		return false;

	if(len < m_fieldes[col].max_length )
		return false;

	if( !is_valid_col(col) )
		return false;

	memcpy_s(value, len, m_cur_row[col], m_fieldes[col].max_length);
	return true;
}

bool ndb_query_result::get_real_value_blob(const char* col_name, char* value, uint32 len)
{
	return get_real_value_blob(get_col_by_name(col_name), value, len);
}


EFieldType ndb_query_result::get_col_type(uint32 col)
{
	if( !is_valid_col(col) ) return EFType_UNKNOWN;


	switch (m_fieldes[col].type)
	{
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_DATE:
	case FIELD_TYPE_TIME:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_SET:
	case FIELD_TYPE_NULL:
		return EFType_STRING;

	case FIELD_TYPE_TINY:
		return EFType_BYTE;

	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_ENUM:
		return EFType_INTEGER;

	case FIELD_TYPE_LONGLONG:
		return EFType_BIGINT;

	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return EFType_FLOAT;


	case FIELD_TYPE_BLOB:
		return EFType_BLOB;
	}

	return EFType_UNKNOWN;
}


EFieldType ndb_query_result::get_col_type(const char* col_name)
{
	int32 col = get_col_by_name(col_name);
	if( col>-1 )
		return get_col_type(col);
	return EFType_UNKNOWN;
}


bool ndb_query_result::is_valid_col(uint32 col)
{
	bool valid_col = ( (col>=0) && (col<m_cols) && (m_cur_row!=NULL) );
	assert(valid_col);
	return valid_col;
}


int32 ndb_query_result::get_col_by_name(const char* col_name)
{
	if(!m_mysql_res || !m_cur_row || !m_fieldes)
		return -1;

	if(col_name)
	{
		for(uint32 col=0; col<m_cols; ++col)
		{
			if( 0== memcmp(col_name, m_fieldes[col].name, strlen(col_name)) )
			{
				return col;
			}
		}
	}
	return -1;
}


void ndb_query_result::_init(MYSQL_RES* mysql_res, uint32 rows, uint32 cols)
{
	m_cols		= cols;
	m_rows		= rows;
	m_cur_row	= NULL;
	m_fieldes	= NULL;
	m_mysql_res = mysql_res;

		if(m_mysql_res) m_fieldes	= mysql_fetch_fields(m_mysql_res);
	}

void ndb_query_result::_un_connect(void)
{
	if( m_mysql_res )
	{
		mysql_free_result(m_mysql_res);
	}

	_init(NULL, 0, 0);
}

} //namespace nexus