/**
 *	nexus nperfmon_test - nunit
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NUNIT_H_
#define _NPERFMON_TEST_NUNIT_H_

#include "ncommon.h"
#include "nworld_object.h"
#include "nobject_fields.h"

using namespace gameframework;

namespace nexus {

	class nmap;

	class nunit : public nworld_object
	{
	public:
		nunit();
		virtual ~nunit();

		bool init();
		virtual void destroy();
		virtual void update(uint32 elapse);

		nobj_data& get_data() 				{ return m_data; }

		bool has_flag(uint32 flag) const									{ return m_data.has_flag(EUF_UnitFlags, flag); }

	protected:
		nobj_data			m_data;
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NUNIT_H_
