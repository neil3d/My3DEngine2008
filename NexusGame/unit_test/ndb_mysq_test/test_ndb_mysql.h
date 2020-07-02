/**
 *	nexus  - ndb_mysql_test - test_ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _TEST_NDB_MYSQL_
#define _TEST_NDB_MYSQL_


#include "ndb_mysql/ndb_query_stream.h"
#include "ndb_mysql/ndb_query_result.h"
#include "ndb_mysql/ndb_mysql.h"
#include <boost/function.hpp>

namespace nexus{


	/**
	 *	ntest_ndb_mysql
	 */
	class ntest_ndb_mysql
	{
	public:
		ntest_ndb_mysql(void){}
		~ntest_ndb_mysql(void){}

		// 检测简单连接
		bool test_connect_to_db_server(void);

		// 检测指定数据库中的表
		void simple_select_tables_in_db(void);

		// 简单执行select
		void simple_test_select(void);

		// 简单执行insert
		void simple_test_insert(void);

		// 简单测试 ndb_query_result重载[]运算符
		void test_qurey_result_operator(void);

		// 简单测试 get_real_value()
		void simple_test_get_value_template(void);

		// 简单测试一个数据存储
		void simple_test_procedure(void);

		// 简单检测sql拼串
		void simple_test_query_stream(void);

		void simple_test_thread(void);

		// 简单测试 ndb_query_result测试以列名称查找接口
		void test_qurey_result_operator_name(void);

		// 检测ndb_query_stream的<<操作运算符
		void test_query_stream_out_operator();

		// 检测拼写blob块
		void test_spell_blob(void);

		// 用于测试数据库执行不同操作不同数量级别的数据的性能
		void test_insert_level(void);
		void test_update_level(void);
		void test_select_level(void);

		// 测试有无索引的差异
		void test_index_select(void);

		// 总的检测函数
		static void test(void);

	private:
		//遍历查询返回结果集
		void iterator_query_result(void);

		// 输出col对应的列值
		void out_to_screen(uint32 col);

		std::string filter_sql_format(char* format, ...);

		void _test_insert(uint32 start_data_level, uint32 end_data_level);
		void _test_update(void);
		void _test_select(uint32 rows);
		void _test_index_select(ndb_query_stream& stream);

		void calc_opration_time(boost::function<void()> sql_test_func);

	private:
		ndb_mysql			m_test_ndb;
		ndb_query_result	m_query_result;
		ndb_query_stream	m_query_stream;
	};

} // namespace nexus

#endif	_TEST_NDB_MYSQL_