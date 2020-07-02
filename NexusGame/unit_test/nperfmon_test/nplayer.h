/**
 *	nexus nperfmon_test - nplayer
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NPLAYER_H_
#define _NPERFMON_TEST_NPLAYER_H_

#include "ncommon.h"
#include "nunit.h"

namespace nexus {

	class nplayer : public nunit
	{
	public:
		nplayer();
		virtual ~nplayer();

		bool init();
		virtual void destroy();
		virtual void update(uint32 elapse);

	private:
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NPLAYER_H_
