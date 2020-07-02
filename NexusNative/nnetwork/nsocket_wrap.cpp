#include "nsocket_wrap.h"
#include "../ncore/ncore.h"

#pragma warning(disable:4503)	// warning : decorated name length exceeded, name was truncated

namespace nexus
{
	nsocket_wrap::nsocket_wrap(boost::asio::io_service& asio_obj, size_t max_pkt_size)
		: m_socket(asio_obj),
		m_strand(asio_obj),
		m_max_pkt_size(max_pkt_size),		
		m_pkt_pool(max_pkt_size),
		m_send_pkt( m_pkt_pool.alloc_pkt(), max_pkt_size )
	{
	}

	nsocket_wrap::~nsocket_wrap(void)
	{
	}

	void nsocket_wrap::send_push_msg(nnet_msg_header* msg)
	{
		boost::recursive_mutex::scoped_lock lock(m_send_mutex);
		m_send_pkt.push_msg(msg);
	}

	void nsocket_wrap::send_packet()
	{
		boost::recursive_mutex::scoped_lock lock(m_send_mutex);
		bool start = m_send_queue.empty();

		if( !m_send_pkt.empty() )
		{
			m_send_queue.push(m_send_pkt.pkt_ptr);
			m_send_pkt.reset(m_pkt_pool.alloc_pkt(), m_max_pkt_size);

			//-- 如果之前未发起过async write，则发起一次
			//	async write发起之后，会通过完成事件连续的将发送队列中的包全部发送完毕
			if (start)
				async_send_packet();
		}
	}

	void nsocket_wrap::async_read_packet_header()
	{
		m_recv_ptr = m_pkt_pool.alloc_pkt();

		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_recv_ptr.get(), sizeof(nnet_packet_header)),
			boost::asio::transfer_at_least(sizeof(nnet_packet_header)),
			m_strand.wrap(
			boost::bind(&nsocket_wrap::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)			
			)
			);
	}

	void nsocket_wrap::handle_read_header(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if(error)
		{
			handle_error(error);
			return;
		}

		nASSERT(sizeof(nnet_packet_header) == bytes_transferred);

		async_read_packet_body();
	}

	void nsocket_wrap::async_read_packet_body()
	{
		//-- 检查包信息
		nASSERT( m_recv_ptr );

		nnet_packet_header* pkt = m_recv_ptr.get();
		size_t body_size = pkt->pkt_size-sizeof(nnet_packet_header);

		nASSERT(body_size+sizeof(nnet_packet_header) <= m_max_pkt_size);

		if( body_size+sizeof(nnet_packet_header) > m_max_pkt_size )
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
			m_strand.wrap(
				boost::bind(&nsocket_wrap::handle_read_packet, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)			
				)
			);
	}

	void nsocket_wrap::handle_read_packet(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if(error)
		{
			handle_error(error);
			return;
		}

		if( m_on_recv )
			m_socket.get_io_service().post( boost::bind(m_on_recv, m_recv_ptr) );

		async_read_packet_header();
	}

	void nsocket_wrap::async_send_packet()
	{
		//-- 此处无需lock send mutex，已由上层调用函数lock
		nnet_packet_ptr pkt = m_send_queue.front();

		nnet_packet_header* pkt_header = pkt.get();

		boost::asio::async_write(m_socket,
			boost::asio::buffer(pkt.get(), pkt_header->pkt_size),
			boost::asio::transfer_at_least(pkt_header->pkt_size),			
			m_strand.wrap(
				boost::bind(&nsocket_wrap::handle_send_packet, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)							
				)
			);	
	}

	void nsocket_wrap::handle_send_packet(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if(error)
		{
			handle_error(error);
			return;
		}

		boost::recursive_mutex::scoped_lock lock(m_send_mutex);

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

	void nsocket_wrap::close()
	{
		if(m_socket.is_open())
		{
			boost::system::error_code ec;
			m_socket.shutdown(tcp::socket::shutdown_both, ec);
			m_socket.close(ec);
		}
	}

	void nsocket_wrap::handle_error(const boost::system::error_code& error)
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
}//namespace nexus