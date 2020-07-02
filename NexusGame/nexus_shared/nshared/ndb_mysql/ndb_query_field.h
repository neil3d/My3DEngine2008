/**
 *	nexus nshared - ndb_query_field
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_QUERY_FIELD_H_
#define _NSHARED_NDB_QUERY_FIELD_H_

#include "ndb_utility.h"

namespace nexus{

	//列的数据类型枚举
	enum EFieldType
	{
		EFType_UNKNOWN	= 0,			// 未知
		EFType_INTEGER	= 1,			// 整型
		EFType_BIGINT	= 2,			// 64长整型
		EFType_STRING	= 3,			// 字符串型
		EFType_FLOAT	= 4,			// 浮点型
		EFType_BLOB		= 5				// 二进制类型
	};

	/**
	 *	ndb_query_field
	 */
	class ndb_query_field
	{
	public:
		ndb_query_field(void): m_value(NULL), m_name(NULL), m_len(0), m_max_len(0), m_field_type(EFType_UNKNOWN)
		{
		}

		~ndb_query_field(void)
		{
		}

		void set_all(char* name, EFieldType field_type, uint32 max_len);

		void set_value_info(char* value)	{	m_value		= value;		}
		void set_name(char* name)			{	m_name		= name;			}
		void set_len(uint32 len)			{	m_len		= len;			}
		void set_type(EFieldType filed_type){	m_field_type= filed_type;	}
		void set_max_len(uint32 max_len)	{	m_max_len	= max_len;		}


		const char* get_value_info(void){	return	m_value;	}
		const char* get_name(void)		{	return	m_name;		}
		uint32		get_len(void)		{	return	m_len;		}
		uint32		get_max_len(void)	{	return	m_max_len;	}
		EFieldType	get_filed_type(void){	return	m_field_type;}


		/**
		 *	获取实际该列的值
		 *	@remark 注意和get_value_byte()的区别
		 */
		template<class T> T get_value()
		{
			return boost::lexical_cast<T>(std::string(m_value));
		}

		/**
		 *	获取实际该列的值
		 *	@remark 该列的真实值类型为byte
		 */
		int8 get_value_byte()
		{
			return  static_cast<int8> ( boost::lexical_cast<int32>(std::string(m_value)) );
		}

	private:
		char*		m_value;		//列值
		char*		m_name;			//列名
		uint32		m_len;			//列长
		uint32		m_max_len;		//最大列长
		EFieldType	m_field_type;	//列数据类型
	};

} // namespace nexus

#endif	_NSHARED_NDB_QUERY_FIELD_H_