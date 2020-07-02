// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



#include "prerequisites.h"

// TODO: reference additional headers your program requires here
//-- boost
#pragma warning(push)
#pragma warning(disable:4512)
#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "ncore.h"
using namespace nexus;