
#include <iostream>
#include <hash_map>

#include "nserver.h"
#include "nworld.h"
#include "nsession_mgr.h"
#include "nlog_mt.h"
#include "nmutex.h"
#include "nguard.h"
#include "nlog_mt.h"

#include <signal.h>
#include <assert.h>

using namespace nexus;

// 1. 测试服务器关闭
//int main(int argc, char* argv[])
//{
//	//tagNS_Chat msg;
//
//	//std::string str = "ni hao, hello";
//	//memcpy(msg.msg, str.c_str(), str.size());
//
//	ntcp_server_config param;
//
//	g_nserver.startup(param);
//
//	// nserver svr(g_nserver);
//	// nserver svr = g_nserver;
//
//	int n = 50;
//
//	while (true)
//	{
//		--n;
//
//		if (0 == n)
//		{
//			break;
//		}
//
//		std::cout << "."; 
//		::Sleep(200);
//	}
//
//	g_nserver.shutdown();
//	
//	::Sleep(3000);
//	return 0;
//}
//
bool g_stop = false;

void _OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
	case SIGBREAK:
		g_stop = true;
		break;
	}

	signal(s, _OnSignal);
}

void _HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
	signal( SIGBREAK, _OnSignal );
}


void _UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
	signal( SIGBREAK, 0 );
}

int main(int argc, char* argv[])
{
	//nmutex mutex;
	//nguard<nmutex> guard(mutex);

	//tagNS_Chat msg;

	//std::string str = "ni hao, hello";
	//memcpy(msg.msg, str.c_str(), str.size());
	int64	i64_test;
	int32	i32_test;
	int16	i16_test;
	int8	i8_test;

	uint64	ui64_test;
	uint32	ui32_test;
	uint16	ui16_test;
	uint8	ui8_test;

	bool	bool_test;
	float	float_test;
	double	double_test;
	char	char_test;

	nchar	nchar_test;

	assert(8 == sizeof(int64) && 8 == sizeof(i64_test));
	assert(4 == sizeof(int32) && 4 == sizeof(i32_test));
	assert(2 == sizeof(int16) && 2 == sizeof(i16_test));
	assert(1 == sizeof(int8) && 1 == sizeof(i8_test));

	assert(8 == sizeof(uint64) && 8 == sizeof(ui64_test));
	assert(4 == sizeof(uint32) && 4 == sizeof(ui32_test));
	assert(2 == sizeof(uint16) && 2 == sizeof(ui16_test));
	assert(1 == sizeof(uint8) && 1 == sizeof(ui8_test));

	assert(1 == sizeof(bool) && 1 == sizeof(bool_test));
	assert(4 == sizeof(float) && 4 == sizeof(float_test));
	assert(8 == sizeof(double) && 8 == sizeof(double_test));
	assert(1 == sizeof(char) && 1 == sizeof(char_test));
	assert(2 == sizeof(nchar) && 2 == sizeof(nchar_test));

	//// 测试
	//nclient_id client1, client2;
	//client1.session_id = 1;
	//client2.session_id = 2;

	/*stdext::hash_map<nclient_id, int32, _cmp_opaque> map_test;
	map_test.insert(std::make_pair(client1, 1));
	map_test.insert(std::make_pair(client1, 2));*/

	DWORD new_size = 0x0010;
	DWORD head = new_size << 1;
	DWORD head2 = (new_size << 1) + 1;

	assert(head >> 1 == new_size);
	assert((head2 & 0x00000001) == 1);
	assert(head2 >> 1 == new_size);

	ntcp_server_config param;
	// param.max_session = 2; 连接分配完毕测试

	// 测试session关闭后异步调用send
	// param.max_session = 1;

	if (false == NLOG.init(_T("tcp_server_test"), ELT_Debug, ELT_Debug))
	{
		std::cout << "log init error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	g_nworld.init(6000);
	g_nsession_mgr.init(4000);

	if (false == g_nserver.startup(param))
	{
		std::cout << "nserver startup error!" << std::endl;
		::Sleep(2000);
		return 1;
	}

	NLOG_STRING(_T("nserver, startup"));
//	std::cout << "nserver startup" << std::endl;

	// nserver svr(g_nserver);
	// nserver svr = g_nserver;
	int32 j = 0;
	_HookSignals();

	while (!g_stop)
	{
// 		std::cout << "."; 
		//if (++j == 500)
		//{
		//	break;
		//}

		::Sleep(200);
	}

	_UnhookSignals();

	g_nserver.shutdown();

	std::cout << "nserver shutdown" << std::endl;

	g_nworld.destroy();
	NLOG.destroy();
	::Sleep(3000);
	return 0;
}
