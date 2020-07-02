/**
*	Network for client
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_CLIENT_NET_IO_H_
#define _NEXUS_CLIENT_NET_IO_H_
#include <string>
#include "conn_define.h"
#include "lib_define.h"
#include "net_packet.h"

namespace nexus
{
	
	/**
	 *	供游戏客户端用的网络模块
	 *	@remark 使用单线程模式；所有接口需要在同一线程(通常是游戏客户端主线程)中调用；
	*/
	class nNET_API nclient_net_io
	{
	public:
		nclient_net_io(void);
		virtual ~nclient_net_io(void);

		/** 异步的连接服务器 */
		void connect_to(const std::string& host_name, const std::string& port, int time_out_sec);
		
		/** 驱动内部IO事件，需要每帧调用*/
		void tick();

		/** 与服务器断开连接 */
		void disconnect();

		/** 向当前的send packet中添加一条message 
		 *	@param msg 需要发送的消息，内部会建立一份拷贝，此函数返回后msg指向的内存可以安全释放
		*/
		void send_push_msg(nnet_msg_header* msg);

		/** 异步的将网络包发送出去*/
		void send_packet();

		/** 是否有接收到的消息 */
		bool recv_empty() const;

		/** 接收队列头部的消息
		 *	@return 返回消息指针或者NULL；外部不应该持有此指针，其内存有nclient_net_io内部管理；
		*/
		nnet_msg_header* recv_front_msg() const;

		/**	弹出消息队列头上的消息
		*/
		void recv_pop_msg();

		/** 设置一个连接状态改变的call back函数
		*/
		void set_state_call_back(conn_state_call_back on_state_change_func);

		/** 得到连接状态 */
		EConnectionState get_state() const;
	private:
		struct impl;
		struct impl*	m_impl;
	};
}//namespace nexus
#endif //_NEXUS_CLIENT_NET_IO_H_