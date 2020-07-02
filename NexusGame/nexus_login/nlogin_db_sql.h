
/**
 *	nexus login - nlogin_db_sql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	July, 2010
 */

#ifndef _NLOGIN_NLOGIN_DB_SQL_H_
#define _NLOGIN_NLOGIN_DB_SQL_H_

#pragma warning(disable : 4996)

namespace nexus{

void _cache_account_info_sprintf(char* sql_sentence, uint32 account_id)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu)","account_cache", account_id);
}

void _exist_account_sprintf(char* sql_sentence,	uint32 account_id)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu)","account_exist", account_id);
}

void _query_psd(ndb_query_stream& query_stream, const nchar* name)
{
	query_stream.spell_select("account", "psd");
	query_stream.set_where()<<"name='";
	query_stream.append_nchar(name, MAX_ACCOUNT_LEN);
	query_stream<<"'";

}

void _query_psd_sprintf(char* sql_sentence, uint32 account_id)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu)","account_psd", account_id);
}

void _fix_login_status(ndb_query_stream& query_stream, uint32 account_id, uint32 status)
{
	query_stream.spell_proce_start("fix_login_status");
	query_stream<<account_id;
	query_stream<<',';
	query_stream<<status;
	query_stream.spell_proce_end();
}

void _fix_login_status_sprintf(char* sql_sentence, uint32 account_id, uint8 status)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu, %d)","fix_login_status", account_id, status);
}

void _get_mibao(ndb_query_stream& query_stream, uint32 account_id, char* mibao)
{
	query_stream.spell_select("account", "mibao");
	query_stream.set_where()<<"id="<<account_id;
}

void _query_privilege(ndb_query_stream& query_stream, const nchar* name)
{
	query_stream.spell_select("account", "id, privilege");
	query_stream.set_where()<<"name=\'";
	query_stream.append_nchar(name, MAX_ACCOUNT_LEN);
	query_stream<<"\'";
}

void _query_privilege_sprintf(char* sql_sentence,	uint32 account_id)
{
	if(!sql_sentence) return;

	sprintf(sql_sentence, "CALL %s(%llu)","account_privilege", account_id);
}


void _login_out(ndb_query_stream& query_stream, uint32 account_id, uint32 last_ip)
{
	query_stream.spell_proce_start("account_login_out");
	query_stream<<account_id;
	query_stream<<',';
	query_stream<<last_ip;
	query_stream.spell_proce_end();
}

void _insert_account_sprintf(char* sql_sentence, naccount_info* data_ptr)
{
	if( !sql_sentence || !data_ptr ) return;

	sprintf(sql_sentence, "CALL %s(\
%llu,\'%s\',\'%s\',%d,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d)","account_insert",\
/*	1	*/	data_ptr->id, data_ptr->name, data_ptr->psd, data_ptr->privilege,
/*	5	*/	data_ptr->last_login, data_ptr->last_ip, data_ptr->login_status,
/*	8	*/	data_ptr->mac[0], data_ptr->mac[1], data_ptr->mac[2], data_ptr->mac[3],
/*	12	*/	data_ptr->mac[4], data_ptr->mac[5], data_ptr->mac[6], data_ptr->mac[7]);
}




void _fix_all_login_status_sprintf(char* sql_sentence, uint8 status)
{
	if( !sql_sentence ) return;

	sprintf(sql_sentence, "CALL %s(%d)","fix_all_login_status", status);
}

}

#endif _NLOGIN_NLOGIN_DB_SQL_H_