#pragma once

#ifdef _DEBUG
	#define D3D_DEBUG_INFO
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX

#include <d3d9.h>
#include <D3DX9Effect.h>
#include <sstream>

#include "base_depend.h"
#include <boost/timer.hpp>
#include "../nengine/renderer/renderer_framework.h"
