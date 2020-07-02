/************************************************************************/
/*  参考boost::asio example/http/server3                                */
/************************************************************************/
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../../NexusNative/ncore/ncore.h"
using namespace nexus;

#include "sample_svr_app.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>

bool g_exit = false;
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		g_exit = true;		
		return TRUE;
	default:
		return FALSE;
	}
}

void test_client_recv(nnet_packet_ptr pkt_ptr)
{
	nnet_pkt_reader pkt_reader(pkt_ptr);
	while( !pkt_reader.empty() )
	{
		nnet_msg_header* msg_header = pkt_reader.front_msg();
		if( msg_header->msg_id == EMSG_Echo )
		{
			MSG_Echo* emsg = (MSG_Echo*)msg_header;
			cout << "test client recv echo msg : " << emsg->msg_txt << endl;
		}
		else
		{
			cout << "test client : BAD msg id." << endl;
		}
		pkt_reader.pop_msg();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		std::cout << "start sample server ... ";
		nlog::instance()->init(_T("sample_svr.log"));

		// Set console control handler to allow server to be stopped.				
		::SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		sample_svr_app my_app;	
		
		if( my_app.startup() )
		{
			std::cout << "ok." << std::endl
				<< "press ctrl+c to exit." << std::endl;
		}
		else
		{
			std::cout << "failed! press any key to exit." << std::endl;
			g_exit = true;
			
			std::cin.get();
		}

		//-- 测试一下服务器组内部用的客户端		
#if 0
		ntcp_client::ptr test_client = my_app.m_svr_net.create_tcp_client();

		ntcp_client_connection::ptr test_conn = test_client->create_connection();
		test_conn->set_recv_handler(&test_client_recv);
		if( test_conn->connect_to("localhost", "9000") )
		{
			MSG_Echo send_msg;
			strcpy(send_msg.msg_txt, "hello form interal client.");

			test_conn->send_push_msg((nnet_msg_header*)&send_msg);
			test_conn->send_packet();
		}
		else
		{
			std::cout << " test client connect failed." << std::endl;
		}
#endif
		//-- 对于反应式服务器，主循环不做任何工作
		//	对于游戏服务，这里应执行游戏主循环
		while(!g_exit)
		{
			::Sleep(1000);
		}

		my_app.shutdown();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	std::cout << "sample server stoped!" << std::endl;
	return 0;
}

#endif // WIN32