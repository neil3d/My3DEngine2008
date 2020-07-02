#include "StdAfx.h"
#include "NGameTimer.h"

#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")

namespace NexusEngine
{
	NGameTimer::NGameTimer(void)
	{
		m_startTime = m_currentTime = m_deltaTime = 0;
	}

	void NGameTimer::Reset()
	{
		m_startTime = timeGetTime();
		m_currentTime = m_startTime;
	}

	void NGameTimer::Tick()
	{
		unsigned int now = timeGetTime();
		m_deltaTime = now - m_currentTime;
		m_currentTime = now;
	}
}//namespace NexusEngine