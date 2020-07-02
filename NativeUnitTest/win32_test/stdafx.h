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
#pragma warning(disable:4127)
#include "base_depend.h"
#include "../../NexusNative/nengine/engine_all.h"
#pragma warning(pop)


//#define ENABLE_LEVEL_SAVE_TEST	// 是否在每个test case创建level完成后，自动保存关卡，以供load测试用