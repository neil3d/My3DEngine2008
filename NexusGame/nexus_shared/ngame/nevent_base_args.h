/**
 *	nexus ngame - nevent_base_args
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NGAME_NEVENT_BASE_ARGS_H_
#define _NGAME_NEVENT_BASE_ARGS_H_

#include "ncommon.h"

namespace nexus {

#pragma pack(push,1)

	struct nevent_base_args
	{
		uint16	event_id;
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NGAME_NEVENT_BASE_ARGS_H_
