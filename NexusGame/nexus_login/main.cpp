#include "nlogin.h"

using namespace nexus; 

int _tmain(int argc, char* argv[])
{
	// try - catch
	if (true == nlogin::instance().init())
	{
		nlogin::instance().run();
	}

	nlogin::instance().destroy();

	// to do: 目前这里增加强退出，在debug版本下，正常退出暂时有问题
	_exit(0);

	return 0;
}
