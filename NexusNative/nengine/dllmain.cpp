// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

namespace nexus
{
	HMODULE g_dll_handle = NULL;
}

FILE* file_stream;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	UNREFERENCED_PARAMETER(lpReserved);
	nexus::g_dll_handle = hModule;
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//调试用的控制台
			/*AllocConsole();
			HANDLE hConsoleInput=GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hConsoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleMode(hConsoleInput,ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
			SetConsoleTextAttribute(hConsoleOutput,FOREGROUND_GREEN);

			freopen_s(&file_stream,"CONOUT$", "a", stdout);*/
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			//FreeConsole();
			break;
		}		
	}
	return TRUE;
}

