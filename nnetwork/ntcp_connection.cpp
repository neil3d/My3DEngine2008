#include "ntcp_connection.h"
#include "nsocket_wrap.h"
#include "nasio_wrap.h"
#include "../ncore/ncore.h"

namespace nexus
{
	ntcp_connection::ntcp_connection(boost::shared_ptr<nasio_wrap> asio_ptr, int id, size_t max_pkt_size)
		: m_socket( new nsocket_wrap(asio_ptr->get_asio(), max_pkt_size) ),
		m_id(id)
	{
	}

	ntcp_connection::~ntcp_connection(void)
	{
		delete m_socket;
	}

	void ntcp_connection::send_push_msg(nnet_msg_header* msg)
	{
		m_socket->send_push_msg(msg);
	}

	void ntcp_connection::send_packet()
	{
		m_socket->send_packet();
	}

	void ntcp_connection::_start()
	{
		m_socket->async_read_packet_header();
	}

	void ntcp_connection::set_recv_handler(recv_handler handler)
	{
		m_socket->set_recv_handler(handler);
	}

	void ntcp_connection::set_state_change_handler(conn_state_call_back	handler)
	{
		m_socket->set_state_change_handler(handler);
	}

	void ntcp_connection::close()
	{
		m_socket->close();
	}
}//namespace nexus