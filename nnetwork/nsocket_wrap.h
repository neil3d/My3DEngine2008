/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/
#ifndef _NEXUS_SOCKET_WRAP_H_
#define _NEXUS_SOCKET_WRAP_H_
#include "lib_define.h"
#include <queue>
#include <boost/asio.hpp>
#include "net_pkt_util.h"
#include "conn_define.h"

#define BOOST_THREAD_DYN_DLL
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

namespace nexus
{
	/**
	 * asio的socket的一个包装，避免暴露asio
	*/
	class nsocket_wrap
	{
	public:
		nsocket_wrap(boost::asio::io_service& asio_obj, size_t max_pkt_size);
		~nsocket_wrap(void);

		tcp::socket& get_socket()	{	return m_socket;}

		/** 主动关闭socket连接	*/
		void close();

		/**	设置网络包接收事件call back函数 */
		void set_recv_handler(recv_handler handler)	{	m_on_recv = handler; }		

		/** 设置状态改变处理函数 */
		void set_state_change_handler(conn_state_call_back	handler) {	m_on_state_change=handler; }

		/** 向当前的网络包中增加消息 */
		void send_push_msg(nnet_msg_header* msg);

		/**	发送当前的网络包 */
		void send_packet();

		/**	发起异步的读请求：读packet header */
		void async_read_packet_header();

		/** 完成packet header读取事件*/
		void handle_read_header(const boost::system::error_code& error, size_t bytes_transferred);
		
		/** 发起异步的读请求：读取packet内容 */
		void async_read_packet_body();

		/** 完成packet内容，即整个packet读完成事件*/
		void handle_read_packet(const boost::system::error_code& error, size_t bytes_transferred);
		
		/** 改变当前状态标示，并调用call back函数 */
		void change_state(EConnectionState st)
		{
			m_state = st;
			if( m_on_state_change )
				m_on_state_change( m_state );
		}
		EConnectionState get_state() const
		{
			return m_state;
		}
	private:
		void async_send_packet();
		void handle_send_packet(const boost::system::error_code& error, size_t bytes_transferred);
		void handle_error(const boost::system::error_code& error);

	private:
		tcp::socket		m_socket;
		recv_handler	m_on_recv;
		
		conn_state_call_back	m_on_state_change;

		// Strand to ensure the connection's handlers are not called concurrently.
		boost::asio::io_service::strand		m_strand;

		EConnectionState		m_state;		

		size_t			m_max_pkt_size;
		nnet_pkt_pool	m_pkt_pool;

		boost::recursive_mutex		m_send_mutex;
		std::queue<nnet_packet_ptr>	m_send_queue;
		nnet_pkt_writer			m_send_pkt;
		nnet_packet_ptr			m_recv_ptr;
	};
}//namespace nexus
#endif //_NEXUS_SOCKET_WRAP_H_