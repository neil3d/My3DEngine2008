/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_TCP_CONNECTION_H_
#define _NEXUS_TCP_CONNECTION_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include "net_packet.h"
#include "conn_define.h"

namespace nexus
{
	class nsocket_wrap;
	class nasio_wrap;

	/**
	 *	与某个client的一条连接
	*/
	class nNET_API ntcp_connection :
		public boost::enable_shared_from_this<ntcp_connection>
	{
	public:
		typedef boost::shared_ptr<ntcp_connection> ptr;

		explicit ntcp_connection(boost::shared_ptr<nasio_wrap> asio_ptr, int id, size_t max_pkt_size);
		virtual ~ntcp_connection(void);

		/** 主动关闭连接 */
		void close();

		/** 向当前的网络包中添加消息 */
		void send_push_msg(nnet_msg_header* msg);
		
		/** 调用socket发送当前的网络包 */
		void send_packet();

		/** 设置网络包接收call back函数 */
		void set_recv_handler(recv_handler handler);

		/** 设置连接状态处理函数 */
		void set_state_change_handler(conn_state_call_back	handler);

		int	get_id() const			{	return m_id; }

	public:
		/** 启动
		 *	@remark 发起一个异步的读取请求
		*/
		void _start();
		nsocket_wrap* _get_socket()	{	return m_socket; }

	protected:
		int				m_id;
		nsocket_wrap*	m_socket;	// asio的socket包装，避免暴露asio

	private:
		// noncopyable
		ntcp_connection(const ntcp_connection&);
		const ntcp_connection& operator = (const ntcp_connection&);
	};
}//namespace nexus

#endif //_NEXUS_TCP_CONNECTION_H_