#include <map>
#include "nasio_wrap.h"
#include "nsocket_wrap.h"
#include "ntcp_svr.h"
#include "ntcp_connection.h"
#include "../ncore/ncore.h"

using boost::asio::ip::tcp;

namespace nexus
{
	struct ntcp_svr::impl
	{
		boost::shared_ptr<nasio_wrap> m_asio_ptr;	// asio::io_service的包装，避免向外部暴露asio
		tcp::acceptor	m_acceptor;		// 负责侦听
		
		int				m_conn_index;			// 连接流水号，用作connection的id
		size_t			m_max_pkt_size;			// 最大网络包的大小
		ntcp_connection::ptr m_incoming_conn;	// 准备接收新的连接对象

		new_connection_handler	m_new_conn_handler;	// 新连接事件call back

		impl(boost::shared_ptr<nasio_wrap> asio_ptr)
			: m_acceptor(asio_ptr->get_asio()), m_asio_ptr(asio_ptr),
			m_conn_index(0),m_max_pkt_size(NNET_MAX_LAN_PACKET_SIZE)
		{}

		boost::asio::io_service& get_asio()
		{
			return m_acceptor.get_io_service();
		}

		void async_accept()
		{
			ntcp_connection::ptr new_connection( new ntcp_connection(m_asio_ptr, m_conn_index++, m_max_pkt_size) );
			m_acceptor.async_accept(new_connection->_get_socket()->get_socket(),
				boost::bind(&impl::handle_accept, this,
				boost::asio::placeholders::error));

			m_incoming_conn = new_connection;
		}

		void handle_accept(const boost::system::error_code& er)
		{
			if (!er)
			{
				m_incoming_conn->_start();				

				//-- 异步调用new connection的call back函数
				if (m_new_conn_handler)
				{
					m_asio_ptr->get_asio().post(
						boost::bind(m_new_conn_handler,m_incoming_conn)
						);
				}

				async_accept();
			}
			else
			{
				std::string sz_error = er.message();					 
				nLog_Error(_T("ntcp_svr accept error : "), conv_string(sz_error).c_str());
			}
		}

	};

	ntcp_svr::ntcp_svr(boost::shared_ptr<nasio_wrap> asio_ptr)
		: m_impl( new impl(asio_ptr) )
	{
	}

	ntcp_svr::~ntcp_svr(void)
	{
	}

	bool ntcp_svr::startup(const std::string& address, const std::string& port, size_t max_pkt_size)
	{
		m_impl->m_max_pkt_size = max_pkt_size;

		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		tcp::resolver resolver(m_impl->get_asio());
		tcp::resolver::query query(address, port);
		tcp::endpoint endpoint = *resolver.resolve(query);

		tcp::acceptor& acceptor_ = m_impl->m_acceptor; // for short
		boost::system::error_code ec;
		
		acceptor_.open(endpoint.protocol(), ec);
		if(ec)
		{
			nLog_Exception(ec.message().c_str());
			return false;
		}

		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

		acceptor_.bind(endpoint, ec);
		if(ec)
		{
			nLog_Exception(ec.message().c_str());
			return false;
		}

		acceptor_.listen(boost::asio::socket_base::max_connections, ec);
		if(ec)
		{
			nLog_Exception(ec.message().c_str());
			return false;
		}

		m_impl->async_accept();

		return true;
	}

	void ntcp_svr::shutdown()
	{
		m_impl->m_acceptor.close();
	}

	void ntcp_svr::set_new_connection_handler(new_connection_handler handler)
	{
		m_impl->m_new_conn_handler = handler;
	}
}//namespace nexus