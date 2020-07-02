#ifndef _NEXUS_NET_LIB_DEFINE_H_
#define _NEXUS_NET_LIB_DEFINE_H_

#ifdef WIN32
#	pragma warning(disable:4251)	// warning:needs to have dll-interface to be used by clients 

#	ifndef _WIN32_WINNT 
#		define _WIN32_WINNT 0x0501
#	endif

#	ifdef NNETWORK_EXPORTS
#		define nNET_API __declspec(dllexport)
#	else
#		define nNET_API __declspec(dllimport)
#	endif
#endif

#endif