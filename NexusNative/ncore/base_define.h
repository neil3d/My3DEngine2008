/**
*	nexus core - base define
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _BASE_DEFINE_H
#define _BASE_DEFINE_H

namespace nexus
{
	#ifdef WIN32
		#ifdef NCORE_EXPORTS
			#define nCoreAPI __declspec(dllexport)
		#else
			#define nCoreAPI __declspec(dllimport)
		#endif
	#else

	#endif //WIN32


}//namespace nexus
#endif //_BASE_DEFINE_H