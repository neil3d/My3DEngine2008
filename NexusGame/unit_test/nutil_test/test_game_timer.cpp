#include "ncommon.h"

#include "timer/ntimestamp.h"
#include "ntimer.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace nexus;

const uint32 TICK_TIME = 80;

BOOST_AUTO_TEST_CASE(test_game_timer)
{
	return;

	ngame_timer g_timer;
	g_timer.init();

	::timeBeginPeriod(1);

	float elapse = 0;
	float cur_time = 0;
	float pre_time = get_time();

	while (true)
	{
		cur_time = get_time();
		elapse = cur_time - pre_time;
		pre_time = cur_time;


		// 注意线程安全
		std::cout << elapse << std::endl;

		cur_time = get_time();

		if (uint32((cur_time - pre_time) * 1000) < TICK_TIME)
		{
			::Sleep(TICK_TIME - uint32((cur_time - pre_time) * 1000));
		}
	}


	ninterval_timer timer;
	timer.set_interval(150);
	return;

	//::timeBeginPeriod(1);

	//uint32 elapse = 0;
	//uint32 cur_time = 0;
	//uint32 pre_time = ::timeGetTime();

	//while (true)
	//{
	//	cur_time = ::timeGetTime();
	//	elapse = cur_time - pre_time;
	//	pre_time = cur_time;


	//	// 注意线程安全
	//	std::cout << elapse << std::endl;
	//	timer.update(elapse);

	//	if (timer.passed())
	//	{
	//		timer.reset();

	//		std::cout << "timer expire" << std::endl;
	//	}

	//	cur_time = ::timeGetTime();

	//	if (cur_time - pre_time < TICK_TIME)
	//	{
	//		::Sleep(TICK_TIME - cur_time + pre_time);
	//	}
	//}




	uint32 realCurrTime = 0;
	uint32 realPrevTime = ::timeGetTime();

	uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

	///- While we have not World::m_stopEvent, update the world
	while (true)
	{
		realCurrTime = ::timeGetTime();

		uint32 diff = realCurrTime - realPrevTime;

		std::cout << diff << std::endl;
		realPrevTime = realCurrTime;

		if (diff <= 50 + prevSleepTime)
		{
			prevSleepTime = 50 + prevSleepTime - diff;
			::Sleep(prevSleepTime);
		}
		else
			prevSleepTime = 0;

	}


	::timeEndPeriod(1);
}
