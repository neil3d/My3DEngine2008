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

	inline nstring filename_no_extension(const nstring& filename)
	{
		nstring::size_type ipos = filename.find_last_of(_T('.'));

		if (ipos != nstring::npos)
			return filename.substr(0, ipos);
		else
			return filename;
	}
}

#endif //_NEXUS_STRING_UTIL_H_