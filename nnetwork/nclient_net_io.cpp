#include "lib_define.h"
#include <boost/asio.hpp>
#include "nclient_net_io.h"
#include "net_pkt_util.h"
#include "conn_define.h"
#include "../ncore/ncore.h"

using boost::asio::ip::tcp;
using namespace std;

#pragma warning(disable:4503)	// warning : decorated name length exceeded, name was truncated

namespace nexus
{
	struct nclient_net_io::impl
	{
		//-- boost::asio objects
		boost::asio::io_service		m_asio;
		tcp::resolver				m_resolver;
		tcp::socket					m_socket;
		boost::asio::deadline_timer	m_timer;
		
		nnet_pkt_recv_queue			m_recv_queue;		
		EConnectionState			m_state;		
		conn_state_call_back		m_on_state_change;

		nnet_pkt_pool				m_pkt_pool;
		nnet_packet_ptr				m_recv_ptr;		

		std::queue<nnet_packet_ptr>	m_send_queue;
		nnet_pkt_writer				m_send_pkt;
		
		impl(void)
			:m_asio(1),
			m_resolver(m_asio),
			m_socket(m_asio),
			m_timer(m_asio),
			m_state(ECS_Invalid),
			m_pkt_pool(NNET_MAX_WAN_PACKET_SIZE),
			m_send_pkt( m_pkt_pool.alloc_pkt(), NNET_MAX_WAN_PACKET_SIZE )
		{			
		}

		void change_state(EConnectionState st)
		{
			m_state = st;
			if( m_on_state_change )
				m_on_state_change( m_state );
		}

		void async_connect_to(const std::string& host_name, const std::string& port, int time_out_sec)
		{
			// Start an asynchronous resolve to translate the server and service names
			// into a list of endpoints.
			tcp::resolver::query query(host_name, port);
			m_resolver.async_resolve(query,
				boost::bind(&impl::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)				
				);

			m_timer.expires_from_now(boost::posix_time::seconds(time_out_sec));
			m_timer.async_wait(boost::bind(&impl::handle_time_out, this,
				boost::asio::placeholders::error));
		}

		void handle_time_out(const boost::system::error_code& err)
		{
			if( err != boost::asio::error::operation_aborted )
			{
				boost::system::error_code ignore_ec;
				m_socket.close(ignore_ec);
				
				change_state(ECS_ConnectTimeout);
			}
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
					boost::bind(&impl::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator)					
					);
			}
			else
			{
				handle_error(err);
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
				async_read_packet_header();
				change_state(ECS_Connected);
			}
			else if (endpoint_iterator != tcp::resolver::iterator())
			{
				// The connection failed. Try the next endpoint in the list.
				m_socket.close();
				tcp::endpoint endpoint = *endpoint_iterator;
				m_socket.async_connect(endpoint,					
					boost::bind(&impl::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator)					
					);
			}
			else
			{
				handle_error(err);
			}
		}

		void async_read_packet_header()
		{
			m_recv_ptr = m_pkt_pool.alloc_pkt();

			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_recv_ptr.get(), sizeof(nnet_packet_header)),
				boost::asio::transfer_at_least(sizeof(nnet_packet_header)),				
				boost::bind(&impl::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)							
				);
		}

		void handle_read_header(const boost::system::error_code& error, size_t bytes_transferred)
		{
			if(error)
			{
				handle_error(error);
				return;
			}

			nASSERT(sizeof(nnet_packet_header) == bytes_transferred);
			
			//-- 检查包信息
			nASSERT( m_recv_ptr );

			nnet_packet_header* pkt = m_recv_ptr.get();
			size_t body_size = pkt->pkt_size-sizeof(nnet_packet_header);

			nASSERT(body_size+sizeof(nnet_packet_header) <= NNET_MAX_WAN_PACKET_SIZE);

			if( body_size+sizeof(nnet_packet_header) > NNET_MAX_WAN_PACKET_SIZE )
			{
				nLog_Error(_T("nclient_net_io error: recv bad packet size"));
				change_state( ECS_Error );
				return;
			}

			//-- 读请求
			char* pkt_buffer = (char*)pkt;
			boost::asio::async_read(m_socket,
				boost::asio::buffer(pkt_buffer+sizeof(nnet_packet_header), body_size),
				boost::asio::transfer_at_least(body_size),				
				boost::bind(&impl::handle_read_packet, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)							
				);
		}

		void handle_read_packet(const boost::system::error_code& error, size_t bytes_transferred)
		{
			if(error)
			{
				handle_error(error);
				return;
			}

			m_recv_queue.append_packet( m_recv_ptr );
			m_recv_ptr.reset();

			async_read_packet_header();
		}

		void handle_error(const boost::system::error_code& error)
		{
			std::string sz_error = error.message();
			nLog_Error(_T("net socket error: %s \r\n"),  conv_string(sz_error).c_str());

			if (error == boost::asio::error::eof)
			{
				change_state( ECS_Disconnected );
			}
			else
			{
				change_state( ECS_Error );
			}		
		}

		void send_push_msg(nnet_msg_header* msg)
		{			
			m_send_pkt.push_msg(msg);
		}

		void send_packet()
		{			
			bool start = m_send_queue.empty();

			if( !m_send_pkt.empty() )
			{
				m_send_queue.push(m_send_pkt.pkt_ptr);
				m_send_pkt.reset(m_pkt_pool.alloc_pkt(), NNET_MAX_WAN_PACKET_SIZE);
				
				if (start)
					async_send_packet();
			}
		}

		void async_send_packet()
		{
			//-- 此处无需lock send mutex，已由上层调用函数lock
			nnet_packet_ptr pkt = m_send_queue.front();

			nnet_packet_header* pkt_header = pkt.get();

			boost::asio::async_write(m_socket,
				boost::asio::buffer(pkt.get(), pkt_header->pkt_size),
				boost::asio::transfer_at_least(pkt_header->pkt_size),							
				boost::bind(&impl::handle_send_packet, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)							
				);	
		}

		void handle_send_packet(const boost::system::error_code& error, size_t bytes_transferred)
		{
			if(error)
			{
				handle_error(error);
				return;
			}			

			nnet_packet_ptr sent_pkt = m_send_queue.front();
			nnet_packet_header* pkt_header = sent_pkt.get();

			if(pkt_header->pkt_size != bytes_transferred)
			{
				change_state( ECS_Error );
				nLog_Error(_T("nclient_net_io error: send incomplete.\r\n"));
				return;
			}

			m_send_queue.pop();
			if( !m_send_queue.empty() )
				async_send_packet();
		}

		void close()
		{
			if(m_socket.is_open())
			{
				boost::system::error_code ec;
				m_socket.shutdown(tcp::socket::shutdown_both, ec);
				m_socket.close(ec);
			}
		}

	};

	nclient_net_io::nclient_net_io(void) 
		: m_impl(nNew impl)
	{
	}

	nclient_net_io::~nclient_net_io(void)
	{
		delete m_impl;
	}

	void nclient_net_io::connect_to(const std::string& host_name, const std::string& port, int time_out_sec)
	{
		m_impl->async_connect_to(host_name, port, time_out_sec);
		m_impl->change_state(ECS_Connecting);
	}
	
	void nclient_net_io::disconnect()
	{
		m_impl->close();
	}

	void nclient_net_io::tick()
	{
		boost::system::error_code ec;
		m_impl->m_asio.poll(ec);
	}

	void nclient_net_io::set_state_call_back(conn_state_call_back on_state_change_func)
	{
		m_impl->m_on_state_change = on_state_change_func;
	}

	void nclient_net_io::send_push_msg(nnet_msg_header* msg)
	{
		m_impl->send_push_msg(msg);			
	}
	
	void nclient_net_io::send_packet()
	{
		m_impl->send_packet();
	}

	bool nclient_net_io::recv_empty() const
	{
		return (m_impl->m_recv_queue.empty() );
	}

	nnet_msg_header* nclient_net_io::recv_front_msg() const
	{
		return m_impl->m_recv_queue.front_msg();
	}

	void nclient_net_io::recv_pop_msg()
	{
		m_impl->m_recv_queue.pop_msg();
	}

	EConnectionState nclient_net_io::get_state() const
	{
		return m_impl->m_state;
	}
}//namespace nexus