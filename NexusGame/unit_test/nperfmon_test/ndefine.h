/**
 *	nexus nperfmon_test - ndefine
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NDEFINE_H_
#define _NPERFMON_TEST_NDEFINE_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"

namespace nexus {

	enum
	{
		CI_Begin		= Msg_World_Login + 1,

		CI_Connect,
		CI_DisConnect,
		CI_Command,

		CI_End			= 1950,
	};

	CMD_START(CI_Connect)
		uint32			session_id;
		uint32			param1;
		uint32			param2;
	CMD_END

	CMD_START(CI_DisConnect)
		uint32			session_id;
	CMD_END

	CMD_START(CI_Command)
		nchar			cmd[256];
	CMD_END

} // namespace nexus

#endif // _NPERFMON_TEST_NDEFINE_H_
