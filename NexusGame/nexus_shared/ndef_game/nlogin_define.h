/**
 *	nexus ndef_game - nlogin_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NDEF_GAME_NLOGIN_DEFINE_H_
#define _NDEF_GAME_NLOGIN_DEFINE_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

//枚举各种常量
enum
{
	//账号长度
	MAX_ACCOUNT_LEN		= 32,
	MAX_PASSWORD_LEN	= 32,
	MAX_MIBAO_LEN		= 6,
	MAX_MD5_SIZE		= 16,
};


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NLOGIN_DEFINE_H_