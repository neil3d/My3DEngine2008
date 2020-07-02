#include "nperfmon_test.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == nperfmon_test::instance().init())
	{
		nperfmon_test::instance().run();
	}

	nperfmon_test::instance().destroy();
	return 0;
}
