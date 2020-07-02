// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include "base_depend.h"
#include <boost/timer.hpp>
#include "ncore.h"
#include "../NexusNative/nengine/engine_all.h"
#pragma warning(pop)
