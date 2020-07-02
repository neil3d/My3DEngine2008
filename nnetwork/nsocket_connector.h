/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_SOCKET_CONNECTOR_H_
#define _NEXUS_SOCKET_CONNECTOR_H_
#include <boost/asio.hpp>
#include <boost/function.hpp>

using boost::asio::ip::tcp;

namespace nexus
{
	typedef boost::function<void (boost::system::error_code)>	error_handler;	
	typedef boost::function0<void>	connected_handler;
	typedef boost::function0<void>	time_out_handler;

	/**
	 *	负责完成客户端的异步的socket连接工作
	*/
	class nsocket_connector
	{
	public:
		nsocket_connector(boost::asio::io_service& asio, tcp::socket& skt)
			:m_resolver(asio),
			m_socket(skt),
			m_strand(asio),
			m_timer(asio)
		{}

		~nsocket_connector(void)	{}

		void set_call_back(error_handler on_error, connected_handler on_cted, time_out_handler on_time_out)
		{
			m_on_error = on_error;
			m_on_connected = on_cted;
			m_on_time_out = on_time_out;
		}

		void async_connect_to(const std::string& host_name, const std::string& port, int time_out_sec)
		{
			// Start an asynchronous resolve to translate the server and service names
			// into a list of endpoints.
			tcp::resolver::query query(host_name, port);
			m_resolver.async_resolve(query,
				m_strand.wrap(
				boost::bind(&nsocket_connector::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)
				)
				);

			m_timer.expires_from_now(boost::posix_time::seconds(time_out_sec));
			m_timer.async_wait(boost::bind(&nsocket_connector::handle_time_out, this,
				boost::asio::placeholders::error));
		}

		void handle_resolve(const boost::system::error_code& err,
			tcp::resolver::iterator endpoint_iterator)
		{
			if (!err)
			{
				// Attempt a connection to the first endpoint in the list. Each endpoint
				// will be tried until we successfully establish a connection.
				tcp::endpoint endpoint = *endpoint_iterator;

				m_socket.async_connect(endpoint,
					m_strand.wrap(
					boost::bind(&nsocket_connector::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator)
					)
					);
			}
			else
			{
				m_strand.post(
					boost::bind(m_on_error, err)
					);
			}
		}

		void handle_connect(const boost::system::error_code& err,
			tcp::resolver::iterator endpoint_iterator)
		{
			if (!err)
			{
				//-- The connection was successful.
				m_timer.cancel();

				// disable Nagle algorithom
				tcp::no_delay opt(true);
				m_socket.set_option( opt );

				// open
				m_strand.post( m_on_connected );

			}
			else if (endpoint_iterator != tcp::resolver::iterator())
			{
				// The connection failed. Try the next endpoint in the list.
				m_socket.close();
				tcp::endpoint endpoint = *endpoint_iterator;
				m_socket.async_connect(endpoint,
					m_strand.wrap(
					boost::bind(&nsocket_connector::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator)
					)
					);
			}
			else
			{
				m_strand.post(
					boost::bind(m_on_error, err)
					);
			}
		}
	private:
		void handle_time_out(const boost::system::error_code& err)
		{
			if( err != boost::asio::error::operation_aborted )
			{
				m_socket.close();
				if(m_on_time_out)
					m_on_time_out();
			}
		}
	private:
		tcp::resolver		m_resolver;
		tcp::socket&		m_socket;
		
		boost::asio::deadline_timer	m_timer;

		error_handler		m_on_error;
		connected_handler	m_on_connected;
		time_out_handler	m_on_time_out;

		boost::asio::io_service::strand		m_strand;
	};
}//namespace nexus

#endif //_NEXUS_SOCKET_CONNECTOR_H_