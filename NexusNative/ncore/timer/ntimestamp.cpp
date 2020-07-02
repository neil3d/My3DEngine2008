#include "stdafx.h"

#ifndef STRICT
#	define STRICT
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include "ntimestamp.h"

namespace nexus
{
	static class TimerInit
	{
	public:
		// Public member for a gain of speed.
		::LARGE_INTEGER mTimerFrequency;
		double mStart;

		// Constructor.
		TimerInit()
		{
			/** Does PC specific initialization of timing information */
			if( !::QueryPerformanceFrequency( &mTimerFrequency ) )
			{
				mTimerFrequency.QuadPart = 0;
				mStart = ::timeGetTime() / 1000.0f;
			}
			else
			{
				::LARGE_INTEGER largeCount;
				::QueryPerformanceCounter( &largeCount );

				double count = double( largeCount.QuadPart );
				double frequency = double( mTimerFrequency.QuadPart );

				mStart = count / frequency;
			}
		}
	} g_TimerInit;


	nCoreAPI float get_time()
	{
		// No performance timer available.
		if( g_TimerInit.mTimerFrequency.QuadPart == 0 )
			return float( ::timeGetTime() / 1000.0f - g_TimerInit.mStart );

		::LARGE_INTEGER largeCount;
		::QueryPerformanceCounter( &largeCount );

		double count = double( largeCount.QuadPart );
		double frequency = double( g_TimerInit.mTimerFrequency.QuadPart );

		return float(( count / frequency ) - g_TimerInit.mStart);
	}
}