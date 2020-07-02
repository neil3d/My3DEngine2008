/**
 *	nexus nperfmon_test - ncreature
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NCREATURE_H_
#define _NPERFMON_TEST_NCREATURE_H_

#include "ncommon.h"
#include "nunit.h"

namespace nexus {

	class ncreature : public nunit
	{
	public:
		ncreature();
		virtual ~ncreature();

		bool init();
		void destroy();
		void update(uint32 elapse);


	private:

	};

} // namespace nexus

#endif // _NPERFMON_TEST_NCREATURE_H_
