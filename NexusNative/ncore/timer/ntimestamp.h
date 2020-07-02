#ifndef _NEXUS_CORE_TIMESTAMP_H_
#define _NEXUS_CORE_TIMESTAMP_H_
#include "../base_define.h"
#include "../ntype_def.h"

namespace nexus
{
	/**
	* returns time in seconds. origin is arbitrary.
	* @return time in seconds (arbitrary origin)
	*/
	nCoreAPI float get_time();
}

#endif //_NEXUS_CORE_TIMESTAMP_H_
