#include "ntimer.h"
#include "timer/ntimestamp.h"
#include "ncfg.h"

namespace nexus {

	ngame_timer::ngame_timer() : m_time_stamp(0.0f), m_start_time(0), m_game_time(0)
	{
	}

	ngame_timer::~ngame_timer()
	{
	}

	bool ngame_timer::init()
	{
		m_time_stamp	= nexus::get_time();
		m_game_time		= ::time(NULL);
		m_start_time	= m_game_time;
		return true;
	}

	void ngame_timer::destroy()
	{
	}

	void ngame_timer::update(uint32 elapse)
	{
		m_time_stamp	= nexus::get_time();
		m_game_time		= ::time(NULL);
	}

	ngame_timer g_game_timer;

} // namespace nexus
