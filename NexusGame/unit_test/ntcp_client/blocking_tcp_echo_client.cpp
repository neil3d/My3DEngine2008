//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>

#define _WIN32_WINNT 0x0501

#include <boost/asio.hpp>

#include "../ntcp_server/test_protocol.h"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

void on_datarecv(LPVOID data_ptr, DWORD size)
{
	tagMsgBase* packet = (tagMsgBase*)data_ptr;

	switch (packet->dwMsgID)
	{
	case NS_Chat:
		{
			tagNS_Chat* msg_data_ptr = (tagNS_Chat*)packet;
			std::cout << "recv server msg:" << msg_data_ptr->msg << std::endl;
		}
		break;
	default:
		{
			std::cout << "error msg id" << std::endl;
		}
		break;
	}
}

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "6000");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);

    using namespace std; // For strlen.

	char send_buffer[max_length];
	char recv_buffer[max_length];

	std::size_t msg_size = 0;
	std::string msg = "";

	tagNC_Chat req;

	msg = "nexus network test!";
	memcpy(req.msg, msg.c_str(), msg.length());

	while (true)
	{
		msg_size = sizeof(req);
		*(WORD*)send_buffer = (WORD)msg_size;
		memcpy(send_buffer + 2, &req, msg_size + 2);

		boost::asio::write(s, boost::asio::buffer(send_buffer, msg_size + sizeof(WORD)));


		DWORD msg_head = 0;
		size_t reply_length = boost::asio::read(s, boost::asio::buffer(recv_buffer, sizeof(DWORD)));

		if (reply_length != sizeof(DWORD))
		{
			std::cout << "length error!";
			break;
		}

		msg_head = *(DWORD*)recv_buffer;
		msg_head >>= 1;

		reply_length = boost::asio::read(s, boost::asio::buffer(recv_buffer, msg_head));

		if (reply_length != msg_head)
		{
			std::cout << "length error!";
			break;
		}


		// ½âÎöÏûÏ¢
		DWORD off_size = 0;
		WORD msg_sub_head = 0;

		while (off_size < msg_head)
		{
			msg_sub_head = *(WORD*)(recv_buffer + off_size);
			off_size += sizeof(WORD);
			
			if (off_size + msg_sub_head <= msg_head)
			{
				on_datarecv((recv_buffer + off_size), msg_sub_head);
			}
			else
			{
				break;
			}
	
			off_size += (DWORD)msg_sub_head;
		}

		::Sleep(200);
	}
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
