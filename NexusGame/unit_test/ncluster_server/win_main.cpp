
#include <iostream>

#include "ncserver.h"
#include "nworld.h"
#include "nlog_mt.h"

using namespace nexus;

int main(int argc, char* argv[])
{
	if (false == NLOG.init(_T("ncluster_server_test"), ELT_Debug, ELT_Debug))
	{
		std::cout << "log init error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	g_nworld.init();

	g_ncserver.startup(6010);

	// g_ncserver.startup(6010, 2);		// 2 连接分配完毕测试
	// 测试session关闭后异步调用send
	// g_ncserver.startup(6010, 1);

	std::cout << "ncluster_server startup" << std::endl;

	int32 j = 0;

	while (true)
	{
		//if (++j == 50)
		//{
		//	break;
		//}


//		real_pre_time	= ::GetTickCount();

		::Sleep(1000);
	}

	g_ncserver.shutdown();

	std::cout << "ncluster_server shutdown" << std::endl;

	g_nworld.destroy();
	NLOG.destroy();
	::Sleep(3000);
	return 0;
}
