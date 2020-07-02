
/**
 *	nexus login - nlogin_db_result
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	July, 2010
 */

#ifndef _NLOGIN_NLOGIN_DB_RESULT_H_
#define _NLOGIN_NLOGIN_DB_RESULT_H_

namespace nexus{


//¸ßÐ§
void _cache_account_result_atoi(ndb_query_result& query_result, naccount_info* info_ptr)
{
	char* name	= query_result.get_col_value((uint32)0);
	char* psd	= query_result.get_col_value(1);

	memcpy(info_ptr->name,	name,	MAX_ACCOUNT_LEN);
	memcpy(info_ptr->psd,	psd,	MAX_MD5_SIZE);

	info_ptr->privilege		= query_result.get_uint8(2);
	info_ptr->last_login	= query_result.get_uint32(3);
	info_ptr->last_ip		= query_result.get_uint32(4);
	info_ptr->login_status	= query_result.get_uint8(5);

	info_ptr->mac[0]		= query_result.get_uint8(6);
	info_ptr->mac[1]		= query_result.get_uint8(7);
	info_ptr->mac[2]		= query_result.get_uint8(8);
	info_ptr->mac[3]		= query_result.get_uint8(9);
	info_ptr->mac[4]		= query_result.get_uint8(10);
	info_ptr->mac[5]		= query_result.get_uint8(11);
	info_ptr->mac[6]		= query_result.get_uint8(12);
	info_ptr->mac[7]		= query_result.get_uint8(13);
}


}

#endif _NLOGIN_NLOGIN_DB_RESULT_H_