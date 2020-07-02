/**
 *	nexus nshared - ndb_query_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_QUERY_STREAM_H_
#define _NSHARED_NDB_QUERY_STREAM_H_

#include <assert.h>
#include "ndb_utility.h"

namespace nexus{
	/**
	 *	ndb_query_stream
	 */
	class ndb_query_stream
	{
	public:
		// 分配一个ndb_query_stream对象
		static	ndb_query_stream* alloc_query_stream(void);

		// 回收一个ndb_query_stream对象
		static	void free_query_stream(ndb_query_stream* qurey_stream);

		static std::string filter_format(char* format, ...);

	public:
		ndb_query_stream(void);
		~ndb_query_stream(void);

		// 获取sql语句
		const char* get_centence(void);

		// 生成相关的sql语句
		ndb_query_stream& spell_select(const char* table, const char* seclect);
		ndb_query_stream& spell_update(const char* table);
		ndb_query_stream& spell_insert(const char* table);
		ndb_query_stream& set_replace(const char* table);
		ndb_query_stream& spell_delete(const char* table);

		ndb_query_stream& set_where();

		ndb_query_stream& spell_proce_start(const char* proce);
		ndb_query_stream& spell_proce_end(void);

		/**
		 *	@remark : 这里特别强调在进行 枚举、int8、uint8的数值时，必须先转换为i32类型
		 *	@example
		 *		int8 param = 8;
		 *		ndb_query_stream << ( (int32)(param) );
		 *		主要原因：boost::lexical_cast<std::string>(param)，转换时把param当做char转换，结果不是数值类型
		 */
		template<class T>
		ndb_query_stream& operator << (const T param)
		{
			std::string sql_append = boost::lexical_cast<std::string>(param);
			m_sql_centence.append(sql_append.begin(), sql_append.end());
			return *this;
		}

		ndb_query_stream& append_nchar(const nchar* info, uint32 len);


		// 增加sql语句中的列值对 col = value; 配合 spell_update、spell_insert、spell_replace使用
		template<class T>
		ndb_query_stream& append_col_value(const char* col, T value)
		{
			assert( !is_empty_str(col) );

			if( !m_first_append )
			{
				m_first_append = true;
			(*this) << col << "=" << value;
			}
			else
			{
				(*this) << "," << col << "=" << value;
			}

			return *this;
		}

		//增加sql语句中的列值对 col = value(为字符串类型); 配合 spell_update、spell_insert、spell_replace使用
		ndb_query_stream& append_col_str_value(const char* col, char* value);
		ndb_query_stream& append_col_str_value(const char* col, wchar_t* value);

		/**
		 *	设置select返回从指定位置开始的指定数量的结果集
		 *	@remark			: 该接口配合spell_select
		 *	@param	offset:	: 指定的开始位置
		 *	@param	num		: 指定的数量
		 */ 
		ndb_query_stream& set_limit(int32 offset, int32 num);

		void clear(void);

	private:
		bool is_empty_str(const char* str_ptr){ return ( (NULL==str_ptr) || ('\0'==str_ptr[0]) ); }
		bool is_empty_str(const wchar_t* str_ptr){ return ( (NULL==str_ptr) || (L'\0'==str_ptr[0]) ); }

	private:

		bool		m_first_append;

		std::string	m_sql_centence;
	};

} // namespace nexus

#endif	_NSHARED_NDB_QUERY_STREAM_H_