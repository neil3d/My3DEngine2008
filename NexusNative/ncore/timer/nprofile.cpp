#include "stdafx.h"
#include "../ncore.h"
#include "nprofile.h"

namespace nexus
{
	profile_value::profile_value() : m_lasttime(0),
		m_deltatime(0),m_maxdeltatime(0),
		m_sumtime(0),
		m_count(0),
		m_progress(0)
	{

	}

	void profile_value::start()
	{
		nASSERT(m_progress==0);

		m_lasttime = get_time();

		++m_progress;
	}

	float profile_value::stop()
	{
		nASSERT(m_progress!=0);

		float now = get_time();
		--m_progress;

		m_deltatime = now - m_lasttime;
		m_sumtime += m_deltatime;
		if( m_deltatime > m_maxdeltatime)
		{
			m_maxdeltatime = m_deltatime;
		}

		++m_count;

		return m_deltatime;
	}

	nstring profile_value::to_string()
	{
		// œ‘ ædebug–≈œ¢
		TCHAR str_info[512];
		swprintf( str_info, _T("count: %u sum time:= %.4f max frame time:= %.4f average time:%.4f"),
			m_count, m_sumtime, m_maxdeltatime,get_average());

		return nstring(str_info);
	}
} // end of namespace nexus