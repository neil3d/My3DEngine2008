#include <iostream>
#include "../../nnetwork/nclient_net_io.h"
#include "../sample_svr/sample_protocol.h"
#include "../../ncore/ncore.h"

using namespace std;
using namespace nexus;

void my_call_back(enum EConnectionState st)
{
	switch(st)
	{
	case ECS_Connected:
		cout << "server connected." << endl;
		break;
	case ECS_ConnectTimeout:
		cout << "error : server connect TIME OUT." << endl;
		break;
	case ECS_Disconnected:
		cout << "error : server disconnect." << endl;
		break;
	case ECS_Error:
		cout << "error : server connect error." << endl;
		break;

	default:
		break;
	}
}

int main()
{
	cout << "start sample client ..." << endl;
	cout << "input any line to tick client ,input 'exit' to exit:" << endl;

	nlog::instance()->init(_T("sample_client.log"));
	nclient_net_io client_net;

	client_net.set_state_call_back(my_call_back);
	client_net.connect_to("localhost", "9000", 5);

	//-- 等待建立连接
	while(client_net.get_state()
		== ECS_Connecting)
	{
		client_net.tick();
	}
	
	while(true)
	{
		//-- 驱动底层事件
		client_net.tick();

		//-- 发送消息
		MSG_Echo send_msg;
		cout << ">>";
		cin.getline(send_msg.msg_txt, sizeof(send_msg.msg_txt));

		if( std::string(send_msg.msg_txt) == "exit" )
			break;

		if (strlen(send_msg.msg_txt) > 0)
		{
			client_net.send_push_msg((nnet_msg_header*)&send_msg);
			client_net.send_packet();
		}
		
		//-- 接收消息
		while( !client_net.recv_empty())
		{
			nnet_msg_header* msg_header = client_net.recv_front_msg();
			if( msg_header->msg_id == EMSG_Echo )
			{
				MSG_Echo* emsg = (MSG_Echo*)msg_header;
				cout << "recv echo msg : " << emsg->msg_txt << endl;
			}
			else
			{
				cout << "client : BAD msg id." << endl;
			}
			client_net.recv_pop_msg();
		}
	}

	client_net.disconnect();
	
	cout << "sample client stoped." << endl;
	return 0;
}
