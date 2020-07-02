/**
 *	nexus network - nc_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NNETWORK_NC_SESSION_H_
#define _NNETWORK_NC_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

namespace nexus {

	class nc_server;
	class nc_client;

	/**
	 *	nc_session 网络传输会话，对一个客户端
	 */
	class nNET_API nc_session : private nnoncopyable
	{
	private:
		friend class nc_server;
		friend class nc_client;

	public:
		nc_session();
		virtual ~nc_session();

		/** 
		 * 异步的连接服务器
		 * @param ip_addresss	远程ip地址
		 * @param port			远程端口
		 * @return				连接id,0XFFFFFFFF表示连接请求投递失败
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * 向指定连接发送数据，内部采用引用计数的方式访问数据, 
		 * 如果同一nmessage被send多次要先置所有的引用次数再调用send, server内部会释放nmessage内存数据
		 * note: gateway使用接口
		 * @param session_id	连接id
		 * @param msg_ptr		共享消息的地址
		 * @param size			发送数据的实际长度
		 */
		bool send(uint32 session_id, nmessage* msg_ptr, uint32 size);

		/** 
		 * 向指定连接发送数据，内部会建立一份拷贝，此函数返回后msg指向的内存可以安全释放
		 * note: 游戏逻辑服务器使用接口
		 * @param session_id	连接id
		 * @param msg_ptr		共享消息的地址
		 * @param size			发送数据的实际长度
		 */
		bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size);

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
		// 连接初始化，设置参数，申请资源, 服务器和客户端公用该session通过传递的参数区分server_ptr和client_ptr有且只有一个非空
		bool _init(uint16 index, nc_server* server_ptr, nc_client* client_ptr, uint32 buffer_size, bool nagle);
		// 释放连接申请的内存资源，不包括pool申请资源，pool资源在close时归还
		void _free_memory();
		// 连接在投递acceptex或connectex前数据初始化
		void _pre_accept_or_connect();
		// 连接打开时，初始化socket设置，绑定iocp，投递异步读请求
		void _open(HANDLE completion_port, DWORD address, DWORD port);
		// 归还各种pool资源
		void _release();
		// 关闭指定连接
		void _close();
		// 解析为更小的消息单位
		bool _parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr);

		// 设置session状态
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// 获得session状态
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// 请求连接回调
		void _handle_connect(DWORD last_error);
		// 读数据回调
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr);
		// 写数据回调
		void _handle_write(DWORD last_error, DWORD bytes_transferred);

	private:
		// session close 帮助类，主要在析构函数检查是否释放session pool资源
		class nclose_helper
		{
		private:
			friend class nc_session;

		public:
			nclose_helper(nc_session* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			nc_session*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_id一个连接| 低2字节 表示index | 高2字节表示guid |，0XFFFFFFFF为非法的client_id
		nc_server*			m_server_ptr;					// 上层server指针，用于回调消息
		nc_client*			m_client_ptr;					// 上层client指针，用于回调消息
		SOCKET				m_socket;						// session的socket资源

		noverlappedex2		m_operator;						// 用于acceptex或connectex的重叠结构体，完成键为0，通过地址偏移得到session指针
		noverlappedex		m_read;							// 用于wsarecv的重叠结构体，完成键为session指针
		noverlappedex		m_write;						// 用于wsasend的重叠结构体，完成键为session指针

		nfast_mutex			m_mutex;						// 保护send缓冲区和session状态改变

		TCPSessionStatus	m_status;						// session状态

		DWORD				m_msg_head;						// 消息头
		char*				m_recv_msg_ptr;					// 接收消息
		DWORD				m_read_rd;						// 接收数据的地址偏移指针
		DWORD				m_read_wr;						// 接收数据的地址偏移指针

		char*				m_write_buffer_ptr[2];			// 投递wsasend的数据缓冲区
		DWORD				m_write_rd[2];					// 发送数据的地址偏移指针
		DWORD				m_write_wr[2];					// 发送数据的地址偏移指针
		DWORD				m_write_index;					// 发送缓冲区索引

		volatile LONG		m_wsa_send;						// 异步send标志，1表示正在处理send流程，0表示等待外部调用send请求发送数据
		volatile LONG		m_wsa_recv;						// 异步recv标志，1表示正在处理recv流程，0表示recv流程断开

		DWORD				m_max_buffer_size;				// 接收消息的最大长度
		bool				m_nagle;						// 是否开启nagle算法
	};

} // namespace nexus

#endif // _NNETWORK_NC_SESSION_H_
