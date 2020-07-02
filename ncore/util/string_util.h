/**
*	nexus core - String util function
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_STRING_UTIL_H_
#define _NEXUS_STRING_UTIL_H_
#include <string>
#include "../base_define.h"

namespace nexus
{
	nCoreAPI std::string conv_string(const std::wstring& ws);
	nCoreAPI std::wstring conv_string(const std::string& s);
}

#endif //_NEXUS_STRING_UTIL_H_