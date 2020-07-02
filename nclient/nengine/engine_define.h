#ifndef _NEXUS_ENGINE_DEFINE_H_
#define _NEXUS_ENGINE_DEFINE_H_

#define ENABLE_EDITOR_API

#ifdef WIN32
#	ifdef NENGINE_EXPORTS
#		define nAPI __declspec(dllexport)
#	else
#		define nAPI __declspec(dllimport)
#		endif

//-- EDITOR APIºê
#	ifdef ENABLE_EDITOR_API
#		define nEDAPI nAPI
#	endif
#endif

#endif //_NEXUS_ENGINE_DEFINE_H_