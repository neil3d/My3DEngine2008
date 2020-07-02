/**
*	nexus core - include external lib
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _BASE_DEPEND_H_
#define _BASE_DEPEND_H_

#pragma warning(disable:4819)	//warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
#pragma warning(disable:4251)	//warning C4251: 'nexus::nclass::m_children' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'nexus::nclass'

#define NOMINMAX
#include "memory/nmemory.h"

#include <tchar.h>
#include <math.h>

//-- STL
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <memory>
#include <deque>
#include <limits>

typedef std::wstring	nstring;

//-- boost
#pragma warning(push)
#pragma warning(disable:4512)
#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#define BOOST_THREAD_DYN_DLL 
#include <boost/thread/once.hpp>
#include <boost/thread.hpp>
#pragma warning(pop)

using namespace std;
using namespace boost;

//--
#if defined( _DEBUG )
#	if defined( _MSC_VER ) && !defined( _WIN64 )
#		define nASSERT( x )		if ( !(x)) { _asm { int 3 } }
#	else
#		include <assert.h>
#		define nASSERT assert
#	endif
#else
#	define nASSERT( x ) {}
#endif

#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif


#endif //_BASE_DEPEND_H_