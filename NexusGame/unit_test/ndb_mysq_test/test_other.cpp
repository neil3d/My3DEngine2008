#include <stdio.h>
#include <iostream>
#include <limits>
#include <time.h>
#include <tchar.h>
#include <boost/bind.hpp>



#include "test_other.h"
#include "timer\ntimestamp.h"

#include "nexus_shared\ndef_game\nitem_define.h"
#include "ndb_mysql\ndb_sql_queue.h"
#include "ndb_mysql\ndb_query_stream.h"
#include "ndb_mysql\ndb_query_result.h"
#include "ndb_mysql\ndb_mysql.h"



namespace nexus{



ntest_other::ntest_other(void)
{
	m_db_mysql = new ndb_mysql();
}

ntest_other::~ntest_other(void)
{
	if( m_db_mysql )
	{
		delete m_db_mysql;
		m_db_mysql = NULL;
	}
}


void ntest_other::test(void)
{
char* describle = "\
请输入需要测试的内容\n\
1:测试数据库相关\n\
2:测试map数据结构相关";

	s_util.init();
	ntest_other test_other;

	uint32 test_choose = in_from_console<uint32>(describle);
	if( 1 == test_choose )
	{
		test_other.test_ndb_mysql();
	}
	else if( 2 == test_choose )
	{
		test_other.test_map();
	}
	else
	{
		std::cout << "不做任何测试" << std::endl;
	}
}


// 检测指定数据库中的表
void ntest_other::test_map(void)
{

	for( ; ; )
	{
		MapInt			map_data;
		HashMapInt		hash_map_data;
		UnorderMapInt	unorder_map_data;

		int32		start	= s_util.rand32();
		int32		num		= 100000;
		num = in_from_console<int32>("请输入你希望测试的数量级(0:退出 默认:十万数量级):");
		if( 0 == num ) return;
		if( num<0 )
		{
			std::cout << "输入数量级 <0 ,请重新输入"<<std::endl;
			continue;
		}

		call_test_func( boost::bind(&ntest_other::add_to_map<MapInt>,			this,	&map_data, start, num),			"map insert"		);
		call_test_func( boost::bind(&ntest_other::add_to_map<HashMapInt>,		this,	&hash_map_data, start, num),	"hash_map insert"	);
		call_test_func( boost::bind(&ntest_other::add_to_map<UnorderMapInt>,	this,	&unorder_map_data, start, num),	"unorder_map insert");

		call_test_func( boost::bind(&ntest_other::iterator_map<MapInt>,			this,	&map_data),						"map iterator"		);
		call_test_func( boost::bind(&ntest_other::iterator_map<HashMapInt>,		this,	&hash_map_data),				"hash_map iterator"	);
		call_test_func( boost::bind(&ntest_other::iterator_map<UnorderMapInt>,	this,	&unorder_map_data),				"unorder_map iterator");

		call_test_func( boost::bind(&ntest_other::find_map<MapInt>,				this,	&map_data),						"map find"			);
		call_test_func( boost::bind(&ntest_other::find_map<HashMapInt>,			this,	&hash_map_data),				"hash_map find"		);
		call_test_func( boost::bind(&ntest_other::find_map<UnorderMapInt>,		this,	&unorder_map_data),				"unorder_map find"	);

		call_test_func( boost::bind(&ntest_other::delete_map<MapInt>,			this,	&map_data),						"map delete"		);
		call_test_func( boost::bind(&ntest_other::delete_map<HashMapInt>,		this,	&hash_map_data),				"hash_map delete"	);
		call_test_func( boost::bind(&ntest_other::delete_map<UnorderMapInt>,	this,	&unorder_map_data),				"unorder_map delete");
	}
}


void ntest_other::test_ndb_mysql(void)
{
char* describle = "\
1:检测mysql的小表的insert\n\
2: 检测mysql的大表的insert\n\
3: 检测mysql的select\n\
4: 检测mysql的大表的select\n\
5: 检测mysql的大表的insert，通过不同的存储引擎";	

	m_db_host			= std::string("172.16.11.254");
	m_db_user			= std::string("root");
	m_db_name			= std::string("nexus_login");
	m_db_big_name		= std::string("nexus_game");
	m_table				= std::string("account");
	m_table_cmp			= std::string("account_cmp");
	m_big_table			= std::string("equip");
	m_big_table_cmp		= std::string("equip_cmp");
	m_big_table_myisam	= std::string("equip_myisam");

	uint32 test_choose = in_from_console<uint32>(describle);
	switch(test_choose)
	{
	case 1:
		insert_small_dif_level();
		break;
	case 2:
		insert_big_dif_level();
		break;
	case 3:
		select_dif_level();
		break;
	case 4:
		select_big_dif_level();
		break;
	case 5:
		insert_dif_level_dif_engine();
	default:
		std::cout << "没有指定任何测试" << std::endl;
	}
}

void ntest_other::send_del_sql(ndb_mysql* ndb_sql_ptr, const char* table_name)
{
	if(!ndb_sql_ptr || !table_name) return;

	ndb_query_stream query_stream;
	query_stream.spell_delete(table_name);
	ndb_sql_ptr->excute(query_stream.get_centence());
}

void ntest_other::insert_small_dif_level(void)
{
	if( !connect_to_db(m_db_host.c_str(), m_db_user.c_str(), m_db_name.c_str()) ) return;

	send_del_sql(m_db_mysql, m_table.c_str());
	send_del_sql(m_db_mysql, m_table_cmp.c_str());

	uint32 dis = 5000000; //五百万

	uint32 start_index = 1;
	uint32 end_index = 1;

	uint32 index = 0;
	for( ; ; )
	{
		uint32	insert_num = in_from_console<uint32>("请输入本次需要插入的数量级(0:结束): ");
		if(0==insert_num) return;


		start_index	=	end_index;
		end_index	+=	dis;

		ngenerate_account_id account(insert_num, start_index, end_index);
		uint32* data_ptr = account.get_accounts();
		call_test_func(boost::bind(&ntest_other::_insert_small_common, this, data_ptr, insert_num), "insert_small_common");

		call_test_func(boost::bind(&ntest_other::_test_insert_small_proc, this, data_ptr, insert_num), "insert_small_proc");


	}
}


uint32 ntest_other::get_total_row(ndb_mysql* ndb_sql_ptr, const char* table_name)
{
	if(!ndb_sql_ptr || !table_name) return 0;

	ndb_query_stream query_stream;
	query_stream.spell_select(table_name, "COUNT(*)");

	ndb_query_result query_result;
	if( ndb_sql_ptr->send_query( query_result, query_stream.get_centence() ) )
	{
		query_result.next_row();
		uint32 rows = query_result.get_real_value<uint32>((uint32)0);
		std::cout << table_name << "表的总行数：" << rows << std::endl;
		return rows;
	}

	return 0;
}

void ntest_other::select_dif_level(void)
{
	ndb_mysql cur_mysql;

	if( !connect_to_db(m_db_host.c_str(), m_db_user.c_str(), m_db_name.c_str()) ) return;

	uint32 total_row = get_total_row(&cur_mysql, m_table.c_str());

	for( ; ; )
	{
		uint32	rows = in_from_console<uint32>("请输入本次需要select的行数(0:结束): ");	
		if( rows > total_row )
			rows = total_row;

		if(0 == rows) return;

		call_test_func(boost::bind(&ntest_other::_select_small_dif_level, this, m_table.c_str(), rows), "select_small_dif_level");
	}

}


//大表不同数量级的插入
void ntest_other::insert_big_dif_level(void)
{
	ndb_mysql cur_mysql;

	if( !connect_to_db(m_db_host.c_str(), m_db_user.c_str(), m_db_big_name.c_str()) ) return;

	send_del_sql(m_db_mysql, m_big_table.c_str());
	send_del_sql(m_db_mysql, m_big_table_cmp.c_str());

	uint32 dis = 5000000; //五百万

	uint32 start_index = 1;
	uint32 end_index = 1;

	uint32 index = 0;
	for( ; ; )
	{
		uint32	insert_num = in_from_console<uint32>("请输入本次需要插入的数量级(0:结束): ");
		if(0==insert_num) return;


		start_index	=	end_index;
		end_index	+=	dis;

		ngenerate_account_id account(insert_num, start_index, end_index);
		uint32* data_ptr = account.get_accounts();

		call_test_func(boost::bind(&ntest_other::_insert_big_common, this,  data_ptr, insert_num), "insert_big_dif_level");
		call_test_func(boost::bind(&ntest_other::_test_insert_big_proc_innodb, this, data_ptr, insert_num), "insert_big_dif_level_by_proc");

	}
}

void ntest_other::select_big_dif_level(void)
{
	ndb_mysql cur_mysql;

	if( !cur_mysql.initialize(m_db_host.c_str(), m_db_user.c_str(), NULL, NULL, m_db_big_name.c_str()) )
	{
		std::cout << "连接指定的 "<< m_db_big_name.c_str() << " 据库未成功" << std::endl;
		return;
	}

	uint32 total_row = get_total_row(&cur_mysql, m_big_table.c_str());

	for( ; ; )
	{
		uint32	rows = in_from_console<uint32>("请输入本次需要select的行数(0:结束): ");
		if( rows > total_row )
			rows = total_row;

		if(0 == rows) return;

		call_test_func(boost::bind(&ntest_other::_select_big_dif_level, this, m_big_table.c_str(), rows), "select_big_dif_level");
	}
}

//大表使用不同的存储引擎
void ntest_other::insert_dif_level_dif_engine(void)
{
	if( !connect_to_db(m_db_host.c_str(), m_db_user.c_str(), m_db_big_name.c_str()) ) return;

	send_del_sql(m_db_mysql, m_big_table.c_str());
	send_del_sql(m_db_mysql, m_big_table_myisam.c_str());

	uint32 dis = 5000000; //五百万

	uint32 start_index = 1;
	uint32 end_index = 1;

	uint32 index = 0;
	for( ; ; )
	{
		uint32	insert_num = in_from_console<uint32>("请输入本次需要插入的数量级(0:结束): ");
		if(0==insert_num) return;

		start_index	=	end_index;
		end_index	+=	dis;

		ngenerate_account_id account(insert_num, start_index, end_index);
		uint32* data_ptr = account.get_accounts();

		call_test_func(boost::bind(&ntest_other::_test_insert_big_proc_innodb, this, data_ptr, insert_num), "insert_big_dif_level_by_proc_and_innodb");

		call_test_func(boost::bind(&ntest_other::_test_insert_big_proc_myisam, this, data_ptr, insert_num), "insert_big_dif_level_by_proc_and_myisam");
	}
}


void ntest_other::_test_insert_small_proc(uint32* data_ptr, uint32 num)
{
	ndb_query_stream query_stream;

	for( uint32 index=0; index<num; ++index )
	{
		uint32 account_id = data_ptr[index];

		query_stream.clear();
		query_stream << "CALL test_insert(" << account_id << ",\'" << account_id << "\'," << account_id <<')';

		m_db_mysql->excute(query_stream.get_centence());
	}
}

void ntest_other::_test_insert_big_proc_innodb(uint32* data_ptr, uint32 num)
{
	ndb_query_stream query_stream;

	nequip_data equip_data;
	memset(&equip_data, 10, sizeof(equip_data));
	query_stream.clear();
	for( uint32 index=0; index<num; ++index )
	{
		query_stream.clear();

		equip_data.guid = data_ptr[index];

		query_stream << "CALL test_insert_equip(";

		query_stream << equip_data.guid << "," << equip_data.info_id << "," ;
		query_stream << equip_data.count << ','<< equip_data.slot << ",";
		query_stream <<( (int32)(equip_data.bag) ) << ",";
		query_stream <<( (int32)(equip_data.flag)) << ',';

		query_stream << (int32)(equip_data.quality) << ",";
		query_stream << (int32)(equip_data.level) << "," ;
		query_stream << equip_data.durability << ','<< equip_data.weight << ",";
		query_stream << equip_data.sharp << "," << equip_data.solid << ',' << equip_data.tenacity << ',';

		query_stream << (int32)(equip_data.stat_type[0]) << "," << (int32)(equip_data.stat_type[1]) << ",";
		query_stream << (int32)(equip_data.stat_type[2]) << ',' << (int32)(equip_data.stat_type[3]) << ',';
		query_stream << (int32)(equip_data.stat_type[4]) << "," << (int32)(equip_data.stat_type[5]) << ",";
		query_stream << (int32)(equip_data.stat_type[6]) << ',' << (int32)(equip_data.stat_type[7]) << ',';

		query_stream << equip_data.stat_value[0] << "," << equip_data.stat_value[1] << ",";
		query_stream << equip_data.stat_value[2] << ',' << equip_data.stat_value[3] << ',';
		query_stream << equip_data.stat_value[4] << "," << equip_data.stat_value[5] << ",";
		query_stream << equip_data.stat_value[6] << ',' << equip_data.stat_value[7] << ',';

		query_stream << equip_data.gem[0] << "," << equip_data.gem[1] << "," << equip_data.gem[2] << ',';
		query_stream << equip_data.gem[3] << "," << equip_data.gem[4] << "," << equip_data.gem[5] << ',';


		query_stream << equip_data.spell[0] << "," << equip_data.spell[1] << "," << equip_data.spell[2] << ',' << equip_data.spell[3] << ',';
		query_stream << equip_data.spell_rate[0] << "," << equip_data.spell_rate[1] << "," << equip_data.spell_rate[2] << ',';

		query_stream << equip_data.enchantment << "," << equip_data.creator;

		query_stream << ')';

		m_db_mysql->excute(query_stream.get_centence());
	}
}


void ntest_other::_test_insert_big_proc_myisam(uint32* data_ptr, uint32 insert_num)
{
	ndb_query_stream query_stream;

	nequip_data equip_data;
	memset(&equip_data, 10, sizeof(equip_data));
	query_stream.clear();
	for( uint32 index=0; index<insert_num; ++index )
	{
		query_stream.clear();

		equip_data.guid = data_ptr[index];

		query_stream << "CALL test_insert_equip_myisam(";

		query_stream << equip_data.guid << "," << equip_data.info_id << "," ;
		query_stream << equip_data.count << ','<< equip_data.slot << ",";
		query_stream <<( (int32)(equip_data.bag) ) << ",";
		query_stream <<( (int32)(equip_data.flag)) << ',';

		query_stream << (int32)(equip_data.quality) << ",";
		query_stream << (int32)(equip_data.level) << "," ;
		query_stream << equip_data.durability << ','<< equip_data.weight << ",";
		query_stream << equip_data.sharp << "," << equip_data.solid << ',' << equip_data.tenacity << ',';

		query_stream << (int32)(equip_data.stat_type[0]) << "," << (int32)(equip_data.stat_type[1]) << ",";
		query_stream << (int32)(equip_data.stat_type[2]) << ',' << (int32)(equip_data.stat_type[3]) << ',';
		query_stream << (int32)(equip_data.stat_type[4]) << "," << (int32)(equip_data.stat_type[5]) << ",";
		query_stream << (int32)(equip_data.stat_type[6]) << ',' << (int32)(equip_data.stat_type[7]) << ',';

		query_stream << equip_data.stat_value[0] << "," << equip_data.stat_value[1] << ",";
		query_stream << equip_data.stat_value[2] << ',' << equip_data.stat_value[3] << ',';
		query_stream << equip_data.stat_value[4] << "," << equip_data.stat_value[5] << ",";
		query_stream << equip_data.stat_value[6] << ',' << equip_data.stat_value[7] << ',';

		query_stream << equip_data.gem[0] << "," << equip_data.gem[1] << "," << equip_data.gem[2] << ',';
		query_stream << equip_data.gem[3] << "," << equip_data.gem[4] << "," << equip_data.gem[5] << ',';


		query_stream << equip_data.spell[0] << "," << equip_data.spell[1] << "," << equip_data.spell[2] << ',' << equip_data.spell[3] << ',';
		query_stream << equip_data.spell_rate[0] << "," << equip_data.spell_rate[1] << "," << equip_data.spell_rate[2] << ',';

		query_stream << equip_data.enchantment << "," << equip_data.creator;

		query_stream << ')';

		m_db_mysql->excute(query_stream.get_centence());
	}
}


void ntest_other::_select_big_dif_level(const char* table_name, uint32 rows)
{
	ndb_query_stream query_stream;
	query_stream.spell_select(table_name,  "\
										   guid,info_id,count,slot,bag,flag,\
										   quality,level,durability,weight,sharp,solid,tenacity,\
										   stat_type0,stat_type1,stat_type2,stat_type3,\
										   stat_type4,stat_type5,stat_type6,stat_type7,\
										   stat_value0,stat_value1,stat_value2,stat_value3,\
										   stat_value4,stat_value5,stat_value6,stat_value7,\
										   gem0,gem1,gem2,gem3,gem4,gem5,\
										   spell0,spell1,spell2,spell3,\
										   spell_rate0,spell_rate1,spell_rate2,\
										   enchantment,creator");

	query_stream.set_limit(1, rows);
	ndb_query_result query_result;
	if( m_db_mysql->send_query(query_result, query_stream.get_centence()) )
	{
		query_result.next_row();

		query_result.get_real_value<uint64>((uint32)0);	//guid;	
		query_result.get_real_value<uint32>(1);			//info_id;
		query_result.get_real_value<uint16>(2);			//count;	
		query_result.get_real_value<uint16>(3);			//slot;	
		query_result.get_real_value_byte(4);			//bag;	
		query_result.get_real_value_byte(5);			//flag;	

		query_result.get_real_value_byte(6);			//quality;	
		query_result.get_real_value_byte(7);			//level;		
		query_result.get_real_value<uint32>(8);			//durability;	
		query_result.get_real_value<uint16>(9);			//weight;		
		query_result.get_real_value<uint16>(10);		//sharp;		
		query_result.get_real_value<uint16>(11);		//solid;		
		query_result.get_real_value<uint16>(12);		//tenacity;	

		//stat_type[8]
		query_result.get_real_value_byte(13);
		query_result.get_real_value_byte(14);
		query_result.get_real_value_byte(15);
		query_result.get_real_value_byte(16);
		query_result.get_real_value_byte(17);
		query_result.get_real_value_byte(18);
		query_result.get_real_value_byte(19);
		query_result.get_real_value_byte(20);

		//stat_value[8]
		query_result.get_real_value<uint16>(21);
		query_result.get_real_value<uint16>(22);
		query_result.get_real_value<uint16>(23);
		query_result.get_real_value<uint16>(24);
		query_result.get_real_value<uint16>(25);
		query_result.get_real_value<uint16>(26);
		query_result.get_real_value<uint16>(27);
		query_result.get_real_value<uint16>(28);

		//gem[6]
		query_result.get_real_value<uint16>(29);
		query_result.get_real_value<uint16>(30);
		query_result.get_real_value<uint16>(31);
		query_result.get_real_value<uint16>(32);
		query_result.get_real_value<uint16>(33);
		query_result.get_real_value<uint16>(34);

		//spell[4]
		query_result.get_real_value<uint16>(35);
		query_result.get_real_value<uint16>(36);
		query_result.get_real_value<uint16>(37);
		query_result.get_real_value<uint16>(38);

		//spell_rate[3]
		query_result.get_real_value<uint16>(39);
		query_result.get_real_value<uint16>(40);
		query_result.get_real_value<uint16>(41);

		query_result.get_real_value<uint16>(42);		//enchantment
		query_result.get_real_value<uint64>(43);		//creator
	}
}

void ntest_other::_select_small_dif_level(const char* table_name, uint32 rows)
{
	if( 0==rows ) return;

	ndb_query_stream query_stream;
	query_stream.spell_select(table_name, "id, name,privilege");
	query_stream.set_limit(1, rows);
	ndb_query_result query_result;
	if( m_db_mysql->send_query(query_result, query_stream.get_centence()) )
	{
		query_result.next_row();
		query_result.get_real_value<uint32>((uint32)0);
		query_result.get_col_value(1);
		query_result.get_real_value<uint16>(2);
	}
}


void ntest_other::_insert_small_common(uint32* data_ptr, uint32 insert_num)
{
	for( uint32 index=0; index<insert_num; ++index )
	{
		_send_small_insert_sql(m_db_mysql, data_ptr[index]);
	}
}

void ntest_other::_insert_big_common(uint32* data_ptr, uint32 insert_num)
{
	for( uint32 index=0; index<insert_num; ++index )
	{
		_send_big_insert_sql(m_db_mysql, data_ptr[index]);
	}
}


void ntest_other::_send_small_insert_sql(ndb_mysql* ndb_sql_ptr, uint32 account_id)
{
	if(!ndb_sql_ptr) return;

	char name[12];
	memset(name, '\0', sizeof(name));
	std::string str_name = ndb_query_stream::filter_format("%d", account_id);

	ndb_query_stream query_stream;
	query_stream.spell_insert(m_table.c_str());
	query_stream.append_col_value("id", account_id);
	query_stream.append_col_str_value("name", (char*)(str_name.c_str()));
	query_stream.append_col_value("privilege", account_id);

	ndb_sql_ptr->excute(query_stream.get_centence());
}

void ntest_other::_send_big_insert_sql(ndb_mysql* ndb_sql_ptr, uint32 guid)
{
	if(!ndb_sql_ptr) return;

	char name[12];
	memset(name, '\0', sizeof(name));
	std::string str_name = ndb_query_stream::filter_format("%d", guid);

	uint32 fill_value = 100;

	ndb_query_stream query_stream;
	query_stream.spell_insert(m_big_table.c_str());
	query_stream.append_col_value("guid", guid);
	query_stream.append_col_value("info_id", fill_value);
	query_stream.append_col_value("count", fill_value);
	query_stream.append_col_value("slot", fill_value);
	query_stream.append_col_value("bag", fill_value);
	query_stream.append_col_value("flag", fill_value);


	query_stream.append_col_value("quality", fill_value);
	query_stream.append_col_value("level", fill_value);
	query_stream.append_col_value("durability", fill_value);
	query_stream.append_col_value("weight", fill_value);
	query_stream.append_col_value("sharp", fill_value);
	query_stream.append_col_value("solid", fill_value);
	query_stream.append_col_value("tenacity", fill_value);


	query_stream.append_col_value("stat_type0", fill_value);
	query_stream.append_col_value("stat_value0", fill_value);
	query_stream.append_col_value("stat_type1", fill_value);
	query_stream.append_col_value("stat_value1", fill_value);
	query_stream.append_col_value("stat_type2", fill_value);
	query_stream.append_col_value("stat_value2", fill_value);
	query_stream.append_col_value("stat_type3", fill_value);
	query_stream.append_col_value("stat_value3", fill_value);
	query_stream.append_col_value("stat_type4", fill_value);
	query_stream.append_col_value("stat_value4", fill_value);
	query_stream.append_col_value("stat_type5", fill_value);
	query_stream.append_col_value("stat_value5", fill_value);
	query_stream.append_col_value("stat_type6", fill_value);
	query_stream.append_col_value("stat_value6", fill_value);
	query_stream.append_col_value("stat_type7", fill_value);
	query_stream.append_col_value("stat_value7", fill_value);

	query_stream.append_col_value("gem0", fill_value);
	query_stream.append_col_value("gem1", fill_value);
	query_stream.append_col_value("gem2", fill_value);
	query_stream.append_col_value("gem3", fill_value);
	query_stream.append_col_value("gem4", fill_value);
	query_stream.append_col_value("gem5", fill_value);

	query_stream.append_col_value("spell0", fill_value);
	query_stream.append_col_value("spell1", fill_value);
	query_stream.append_col_value("spell2", fill_value);
	query_stream.append_col_value("spell3", fill_value);

	query_stream.append_col_value("spell_rate0", fill_value);
	query_stream.append_col_value("spell_rate1", fill_value);
	query_stream.append_col_value("spell_rate2", fill_value);

	query_stream.append_col_value("enchantment", fill_value);
	query_stream.append_col_value("creator", fill_value);

	ndb_sql_ptr->excute(query_stream.get_centence());
}

// 连接到指定的数据库
bool ntest_other::connect_to_db(const char* host, const char* user, const char* database)
{
	if(!m_db_mysql) return false;

	if( !m_db_mysql->initialize(host, user, NULL, NULL, database) )
	{
		std::cout << "连接指定的 "<< m_db_big_name.c_str() << " 据库未成功" << std::endl;
		return false;
	}
	return true;
}



void ntest_other::call_test_func(boost::function<void()> test_func, const char* test_info)
{
	if(!test_func) return;

	float last_time = nexus::get_time();
	test_func();
	float end_time = nexus::get_time();

	std::cout << test_info << " : " <<  end_time - last_time << std::endl;
}



} //namespace nexus