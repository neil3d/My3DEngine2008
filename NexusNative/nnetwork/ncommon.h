/**
 *	nexus network - ncommon.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NCOMMON_H_
#define _NNETWORK_NCOMMON_H_

#ifdef WIN32

#	pragma warning(disable:4251)	// warning: needs to have dll-interface to be used by clients 

#	ifndef _WIN32_WINNT 
#		define _WIN32_WINNT 0x0501
#	endif

#	ifdef NDLL
#		ifdef NNETWORK_EXPORTS
#			define nNET_API __declspec(dllexport)
#		else
#			define nNET_API __declspec(dllimport)
#		endif
#	else
#		define nNET_API
#	endif

#define NINLINE __forceinline

#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>
#include <MMSystem.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#endif

#include <assert.h>
#include <time.h>
#include <math.h>

#include <map>
#include <set>
#include <list>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "nnoncopyable.h"
#include "nunicode.h"
#include "nprotocol_base.h"

#endif _NNETWORK_NCOMMON_H_
