/**
 *	nexus nshared - ndb_query_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_QUERY_RESULT_H_
#define _NSHARED_NDB_QUERY_RESULT_H_

#include <assert.h>

#include "ndb_utility.h"

namespace nexus{

	//列的数据类型枚举
	enum EFieldType
	{
		EFType_UNKNOWN	= 0,			// 未知
		EFType_BYTE		= 1,			// 字节型(BYTE)
		EFType_INTEGER	= 2,			// 整型
		EFType_BIGINT	= 3,			// 64长整型
		EFType_STRING	= 4,			// 字符串型
		EFType_FLOAT	= 5,			// 浮点型
		EFType_BLOB		= 6				// 二进制类型
	};

	/**
	 *	ndb_query_result
	 */
	class ndb_query_result
	{
	public:
		ndb_query_result(void);
		~ndb_query_result(void);

		// 获取结果集
		void connect(MYSQL_RES* mysql_res, uint32 rows, uint32 fields);

		bool next_row(void);

		uint32	get_row_count(void){ return m_rows; };
		uint32	get_col_count(void){ return m_cols; };


		// col: 实际的列值(0,1,2,3,...N-1)
		char*	get_col_name(uint32 col);

		// col: 实际的列值(0,1,2,3,...N-1)
		char*	get_col_value(uint32 col);
		char*	get_col_value(const char* col_name);

		// col: 实际的列值(0,1,2,3,...N-1)
		char*	operator[](uint32 col) const;
		char*	operator[](const char* col_name) const;

		// col: 实际的列值(0,1,2,3,...N-1)
		EFieldType get_col_type(uint32 col);
		EFieldType get_col_type(const char* col_name);

		/**
		 *	获取实际该列的数值类值
		 *	@param col: 实际的列值(0,1,2,3,...N-1)
		 *	@remark 注意和get_real_value_byte()、get_real_vlaue_str的区别
		 *	@remark 效率低
		 */
		template<class T> T get_real_value(uint32 col)
		{
			assert( !is_type(col, EFType_BYTE) );

			if( get_col_value(col) )
			return boost::lexical_cast<T>(std::string(get_col_value(col)));

			return (T)(0);
		}

		/**
		 *	@remark 效率低
		 */
		template<class T> T get_real_value(const char* col_name)
		{
			int32 col = get_col_by_name(col_name);

			assert( !is_type(col, EFType_BYTE) );

			if( get_col_value(col) )
			return boost::lexical_cast<T>(std::string(get_col_value(col)));

			return (T)(0);
		}

		/**
		 *	获取指定列的字符信息
		 *	@return : 实际的长度(-1:无效返回)
		 *	@param out value : 输出结果
		 *	@param len : 输出目标的长度
		 */
		int32 get_real_value_str(uint32 col, nchar* value, uint32 len);
		int32 get_real_value_str(const char* col_name, nchar* value, uint32 len);

		/**
		 *	获取实际该列的值
		 *	@param col: 实际的列值(0,1,2,3,...N-1)
		 *	@remark 该列的真实值类型为byte
		 *	@remark 效率低
		 */
		int8 get_real_value_byte(uint32 col)
		{
			assert( is_type(col, EFType_BYTE) );
			return  static_cast<int8> ( boost::lexical_cast<int32>(std::string(get_col_value(col))) );
		}

		/**
		 *	@remark 效率低
		 */
		int8 get_real_value_byte(const char* col_name)
		{
			int32 col = get_col_by_name(col_name);
			return get_real_value_byte(col);
		}

		bool get_real_value_blob(uint32 col, char* value, uint32 len);
		bool get_real_value_blob(const char* col_name, char* value, uint32 len);


		bool is_type(uint32 col, EFieldType type)
		{
			return ( type==get_col_type(col) );
		}

		inline const char* get_string(uint32 col) const;

		inline bool get_bool(uint32 col);
		inline uint32 get_uint32(uint32 col);
		inline int32 get_int32(uint32 col);
		inline uint64 get_uint64(uint32 col);
		inline int8 get_int8(uint32 col);
		inline uint8 get_uint8(uint32 col);
		inline int16 get_int16(uint32 col);
		inline int16 get_uint16(uint32 col);
		inline float get_float(uint32 col);

		inline bool get_bool(const char* col_name);
		inline uint32 get_uint32(const char* col_name);
		inline int32 get_int32(const char* col_name);
		inline uint64 get_uint64(const char* col_name);
		inline int8 get_int8(const char* col_name);
		inline uint8 get_uint8(const char* col_name);
		inline int16 get_int16(const char* col_name);
		inline int16 get_uint16(const char* col_name);
		inline float get_float(const char* col_name);

	private:
		bool is_valid_col(uint32 col);

		int32 get_col_by_name(const char* name);

		void _init(MYSQL_RES* mysql_res, uint32 rows, uint32 cols);

		void _un_connect(void);

	private:
		MYSQL_RES*			m_mysql_res;//mysql结果集
		MYSQL_ROW			m_cur_row;	//当前行
		uint32				m_rows;		//结果行数
		uint32				m_cols;		//结果列数

		MYSQL_FIELD*		m_fieldes;	//所有字段的类型信息
	};

	#include "_ndb_query_result.h"

} // namespace nexus

#endif	_NSHARED_NDB_QUERY_RESULT_H_