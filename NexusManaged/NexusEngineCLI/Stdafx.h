// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

#include <vcclr.h>
#include <gcroot.h>

#include <vector>

#include "nengine/engine_all.h"
#include "nengine/engine_editor.h"
using namespace nexus;

// .Net namespace
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
