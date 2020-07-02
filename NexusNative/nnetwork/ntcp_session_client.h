/**
 *	nexus network - ntcp_session_client
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NTCP_SESSION_CLIENT_H_
#define _NNETWORK_NTCP_SESSION_CLIENT_H_

#include "ncommon.h"
#include "nnetwork_def.h"

#include <string>

namespace nexus {

	class ntcp_client_iocp;
	class nlzo_wrap;

	/**
	 *	ntcp_session_client网络传输会话，对一个连接
	 */
	class nNET_API ntcp_session_client : private nnoncopyable
	{
	private:
		friend class ntcp_client_iocp;

	public:
		ntcp_session_client();
		virtual ~ntcp_session_client();

		/** 
		 * 异步的连接服务器
		 * @param ip_addresss	远程ip地址
		 * @param port			远程端口
		 * @return				连接id,0XFFFFFFFF表示连接请求投递失败
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * 向指定连接发送数据，内部采用引用计数的方式访问数据
		 * 如果同一nmessage被send多次要先置所有的引用次数再调用send, server内部会释放nmessage内存数据
		 * @param session_id	连接id
		 * @param msg_ptr		共享消息的地址
		 * @param size			发送数据的实际长度
		 */
		bool send(uint32 session_id, nmessage* msg_ptr, uint32 size);

		/** 
		 * 关闭指定连接
		 * @param session_id	连接id
		 */
		NINLINE void close(uint32 session_id)
		{
			if (session_id != m_session_id)
			{
				return;
			}

			_close();
		}

	private:
		// 连接初始化，设置参数，申请资源
		bool _init(uint16 index, ntcp_client_iocp* client_ptr, uint32 max_recv_size, uint32 max_send_msg, uint32 max_send_msg_size, uint32 time_out_sec, bool nagle);
		// 释放连接申请的内存资源，不包括pool申请资源，pool资源在close时归还
		void _free_memory();
		// 连接在投递ConnectEx前数据初始化
		void _pre_connect();
		// 归还各种pool资源
		void _release();
		// 关闭指定连接
		void _close();
		// 解析为最小消息单位，回调函数
		bool parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr);

		// 设置session状态
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// 获得session状态
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// 请求连接回调
		void _handle_connect(DWORD last_error);
		// 读数据回调
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr, char* temp_ptr, nlzo_wrap* minilzo_ptr);
		// 写数据回调
		void _handle_write(DWORD last_error, DWORD bytes_transferred);

	private:
		// session close 帮助类，主要在析构函数检查是否释放session pool资源
		class nclose_helper
		{
		private:
			friend class ntcp_session_client;

		public:
			nclose_helper(ntcp_session_client* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			ntcp_session_client*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_id一个连接| 低2字节 表示index | 高2字节表示guid |，0XFFFFFFFF为非法的client_id
		ntcp_client_iocp*	m_client_ptr;					// 上层client指针，用于回调消息
		SOCKET				m_socket;						// session的socket资源

		noverlappedex		m_read;							// 用于wsarecv和connectex的重叠结构体，完成键为session指针
		noverlappedex		m_write;						// 用于wsasend的重叠结构体，完成键为session指针

		nfast_mutex			m_mutex;						// 保护send缓冲区和session状态改变

		TCPSessionStatus	m_status;						// session状态

		DWORD				m_msg_head;						// msg head 打包后的消息头4字节
		bool				m_lzo;							// msg body 是否压缩传输
		char*				m_recv_buffer_ptr;				// 接收消息缓冲区
		DWORD				m_read_rd;						// 接收数据的地址偏移指针
		DWORD				m_read_wr;						// 接收数据的地址偏移指针

		DWORD				m_write_rd;						// 发送数据的地址偏移指针
		DWORD				m_write_wr;						// 发送数据的地址偏移指针

		volatile LONG		m_wsa_send;						// 异步send标志，1表示正在处理send流程，0表示等待外部调用send请求发送数据
		volatile LONG		m_wsa_recv;						// 异步recv标志，1表示正在处理recv流程，0表示recv流程断开

		nmessage*			m_send_msg_ptr;					// 当前发送消息

		nmessage*			m_send_head_ptr;				// 有数据的发送缓冲区头
		nmessage*			m_send_tail_ptr;				// 有数据的发送缓冲区尾

		DWORD				m_cur_send_msg;					// 当前等待发送的消息数目

		DWORD				m_max_recv_size;				// 接收消息的最大长度
		DWORD				m_max_send_msg_size;			// 一次异步wsasend的最大长度
		DWORD				m_max_send_msg;					// 等待发送的最大消息条数
		bool				m_nagle;

		std::string			m_ip_addresss;					// session连接服务器的ip地址
		uint16				m_port;							// session连接服务器的端口
		uint32				m_time_out_sec;					// 请求连接超时时间s
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SESSION_CLIENT_H_
