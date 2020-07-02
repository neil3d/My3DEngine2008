/**
 *	nexus nshared - ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_MYSQL_H_
#define _NSHARED_NDB_MYSQL_H_

#include "ndb_utility.h"

namespace nexus{

	class ndb_query_stream;
	class ndb_query_result;
	class ndb_mysql_queue;

	class ndb_connection
	{
	public:
		nmutex	m_mutex;
		MYSQL*	m_mysql;

		ndb_connection()
		{
			m_mysql = NULL;
		}
	};


	/**
	 *	ndb_mysql
	 */
	class ndb_mysql
	{
	public:
		ndb_mysql(void);
		~ndb_mysql(void);

		// 远程数据库连接
		bool initialize(const char* host, const char* user,const char* port, const char* passwd, const char* db_name);

		// 重新连接数据库
		bool reconnect(void);

		// 关闭数据库
		void shut_down(void);

		/**
		 *	数据库查询相关
		 *	@param query	:	select语句，包括存储过程查询
		 *	@remark	query	:	query不可以为二进制查询语句，只为以'\0'结尾的字符串
		 */
		bool send_query(ndb_query_result& query_result, const char* query);

		/**
		 *	数据库操作相关
		 *	@param opration : sql相关操作语句(包括insert, delete, update, replace)
		 *	@remark			: 和send_query最大的区别是执行sql不需要返回结果集
		 */
		bool excute(const char* query);

		/**
		 *	转换一个blob字符串
		 *	@param out_len : 返回转换后的out_blob的长度
		 *	@remark out_len : 需要传入out_len的值 out_len >= (2*len + 1)
		 */
		bool spell_blob_str(const char* blob, uint32 len, char* out_blob, uint32& out_len);

		// 获取当前使用的数据库名称
		std::string get_db_name(void) { return m_db_name; }

	private:
		// 得到一个空闲连接
		ndb_connection* get_free_connection(void);

		// 使用完后释放一个连接
		void return_connection(ndb_connection* con);

		// 设置数据库连接参数
		void set_cfg(const char* host, const char* user, const char* port, const char* passwd, const char* db_name);

		// 初始化
		void init(void);

		// 发送sql语句前的基本判断
		bool pre_send_query(MYSQL* mysql_ptr, const char* query);


	private:
		// 远程数据库连接
		bool _initialize(void);

		// 执行sql语句
		bool _excute_query(MYSQL* mysql_ptr, const char* query);

		// 指定的连接是否处于连通状态
		bool _is_connect(MYSQL* mysql_ptr);

		// 与mysql相关的错误信息
		void _error_sql_prompt(MYSQL* mysql_ptr, char* error_info);

		// 与mysql无关的错误信息
		void _error_prompt(char* error_info);

	private:
		int32			m_connect_num;
		ndb_connection*	m_connectiones;

		// sql语句队列
		ndb_sql_queue*	m_sql_queue;

		//mysql连接需要的相关参数
		std::string m_host;
		std::string m_user;
		std::string m_db_name;
		std::string m_passwd;
		uint32		m_port;
		uint32		m_client_flag;

		//设置sql客户端字符集
		std::string	m_client_character_set;
		//设置sql连接字符集
		std::string m_connect_character_sql;

	public:
		//获取当前pc的CPU数量
		static uint32 get_cpu_num(void);
	};
} // namespace nexus

#endif	_NSHARED_NDB_MYSQL_H_