#ifndef _NEXUS_GAMEFRAMEWORK_PREREQUISITES_H_
#define _NEXUS_GAMEFRAMEWORK_PREREQUISITES_H_
#include "ncore.h"

namespace gameframework
{
	#ifdef NGAMEFRAMEWORK_EXPORTS
		#define nGameFrameworkAPI DLLEXPORT
	#else
		#ifndef _WIN64
			#define nGameFrameworkAPI DLLIMPORT
		#else
			#define nGameFrameworkAPI
		#endif
	#endif
}

#endif // end of _NEXUS_GAMEFRAMEWORK_PREREQUISITES_H_