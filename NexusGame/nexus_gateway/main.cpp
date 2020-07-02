#include "ngateway.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == ngateway::instance().init())
	{
		ngateway::instance().run();
	}

	ngateway::instance().destroy();
	return 0;
}
