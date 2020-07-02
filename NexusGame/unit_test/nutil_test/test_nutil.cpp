#include "ncommon.h"
#include "nutil.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace nexus;

BOOST_AUTO_TEST_CASE(test_nutil)
{
	s_util.init();


	uint32 uvalue = 0;
	int32 ivalue = 0;
	
	uvalue = s_util.get_cpu_num();
	ivalue = s_util.irand(0, 100);
	uvalue = s_util.urand(0, 100);

	ivalue = s_util.rand32();

	double dvalue = 0.0;
	dvalue = s_util.rand_norm();

	dvalue = s_util.rand_chance();


	bool bvalue = s_util.roll_chance_f(50.0);
	bvalue = s_util.roll_chance_i(50);

	ivalue = 5000;

	while (ivalue > 0)
	{
		uvalue = s_util.urand(0, 100);
		--ivalue;
	}


	uvalue = s_util.crc32("hello", strlen("hello"));
	s_util.destroy();
}
