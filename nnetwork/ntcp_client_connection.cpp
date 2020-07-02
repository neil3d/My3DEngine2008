#include "ntcp_client_connection.h"
#include "nsocket_wrap.h"
#include "nasio_wrap.h"
#include "../ncore/ncore.h"

namespace nexus
{
	ntcp_client_connection::ntcp_client_connection(boost::shared_ptr<nasio_wrap> asio_ptr, int id, size_t max_pkt_size)
		: ntcp_connection(asio_ptr, id, max_pkt_size)
	{
	}

	ntcp_client_connection::~ntcp_client_connection(void)
	{
	}

	bool ntcp_client_connection::connect_to(const std::string& host_name, const std::string& port)
	{
		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(m_socket->get_socket().get_io_service());
		tcp::resolver::query query(host_name, port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		boost::asio::ip::tcp::socket& socket = m_socket->get_socket();
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
		{
			std::string sz_error = error.message();
			nLog_Error(_T("ntcp_client connection failed, error = %s."), conv_string(sz_error).c_str());
			return false;
		}

		// disable Nagle algorithom
		tcp::no_delay opt(true);
		socket.set_option( opt );

		_start();
		return true;
	}
}//namespace nexus