#include "ndb.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == ndb::instance().init())
	{
		ndb::instance().run();
	}

	ndb::instance().destroy();
	return 0;
}