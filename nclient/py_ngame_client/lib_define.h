#ifndef _NEXUS_PY_GAME_CLIENT_H_
#define _NEXUS_PY_GAME_CLIENT_H_

#ifdef WIN32
#	ifdef PY_NGAME_CLIENT_EXPORTS
#		define nClientAPI __declspec(dllexport)
#	else
#		define nClientAPI __declspec(dllimport)
#	endif
#endif

#endif //_NEXUS_PY_GAME_CLIENT_H_