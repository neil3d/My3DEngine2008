#include "nworld.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == nworld::instance().init())
	{
		nworld::instance().run();
	}

	nworld::instance().destroy();
	return 0;
}

