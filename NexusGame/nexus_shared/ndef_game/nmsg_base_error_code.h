/**
 *	nexus ndef_game - nmsg_base_error_code.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_
#define _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_

namespace nexus {

#pragma pack(push,1)

	/**
	 * 所有网络消息共同的错误码, 除了EError_Success，所有错误码均为负数，各个消息独有的错误码可从1开始
	 */
	enum EBaseErrorCode
	{
		EError_Success			= 0,
		EError_SystemError		= -1,

	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_BASE_ERROR_CODE_H_
