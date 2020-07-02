/**
 *	nexus network - ntcp_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_SESSION_H_
#define _NNETWORK_NTCP_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

namespace nexus {

	class ntcp_server;
	class nlzo_wrap;

	/**
	 *	ntcp_session网络传输会话，对一个客户端
	 */
	class nNET_API ntcp_session : private nnoncopyable
	{
	private:
		friend class ntcp_server;

	public:
		ntcp_session();
		virtual ~ntcp_session();

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
		bool _init(uint16 index, ntcp_server* server_ptr, uint32 max_recv_size, uint32 max_write_size, uint32 send_buffers, bool minilzo);
		// 释放连接申请的内存资源，不包括pool申请资源，pool资源在close时归还
		void _free_memory();
		// 连接在投递acceptex前数据初始化
		void _pre_accept();
		// 连接打开时，初始化socket设置，绑定iocp，投递异步读请求
		void _open(HANDLE completion_port, nsafe_mem_pool* pool_ptr, DWORD address, DWORD port);
		// 数据打包
		DWORD _packeted_buffer(char* temp_ptr, DWORD off_size);
		// 归还各种pool资源
		void _release();
		// 关闭指定连接
		void _close();

		// 设置session状态
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// 获得session状态
		NINLINE TCPSessionStatus _get_status() const { return m_status; }

		// 读数据回调
		void _handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr);
		// 写数据回调
		void _handle_write(DWORD last_error, DWORD bytes_transferred, char* temp_ptr, nlzo_wrap* minilzo_ptr);

	private:
		// session close 帮助类，主要在析构函数检查是否释放session pool资源
		class nclose_helper
		{
		private:
			friend class ntcp_session;

		public:
			nclose_helper(ntcp_session* session_ptr) 
				: m_session_ptr(session_ptr) {}
			~nclose_helper();

		private:
			ntcp_session*	m_session_ptr;
		};

	private:
		uint32				m_session_id;					// session_id一个连接| 低2字节 表示index | 高2字节表示guid |，0XFFFFFFFF为非法的client_id
		ntcp_server*		m_server_ptr;					// 上层server指针，用于回调消息
		SOCKET				m_socket;						// session的socket资源

		noverlappedex2		m_accept;						// 用于acceptex的重叠结构体，完成键为0，通过地址偏移得到session指针
		noverlappedex		m_read;							// 用于wsarecv的重叠结构体，完成键为session指针
		noverlappedex		m_write;						// 用于wsasend的重叠结构体，完成键为session指针

		nfast_mutex			m_mutex;						// 保护send缓冲区和session状态改变

		TCPSessionStatus	m_status;						// session状态

		nmessage*			m_recv_msg_ptr;					// 接收消息
		DWORD				m_read_rd;						// 接收数据的地址偏移指针
		DWORD				m_read_wr;						// 接收数据的地址偏移指针

		char*				m_write_buffer_ptr;				// 投递wsasend的数据缓冲区
		DWORD				m_write_rd;						// 发送数据的地址偏移指针
		DWORD				m_write_wr;						// 发送数据的地址偏移指针

		volatile LONG		m_wsa_send;						// 异步send标志，1表示正在处理send流程，0表示等待外部调用send请求发送数据
		volatile LONG		m_wsa_recv;						// 异步recv标志，1表示正在处理recv流程，0表示recv流程断开

		nsend_buffer*		m_send_buffers_ptr;				// 发送的缓冲区

		nsend_buffer*		m_send_head_ptr;				// 有数据的发送缓冲区头
		nsend_buffer*		m_send_tail_ptr;				// 有数据的发送缓冲区尾
		nsend_buffer*		m_empty_head_ptr;				// 空数据的发送缓冲区头

		DWORD				m_max_recv_size;				// 接收消息的最大长度
		DWORD				m_max_write_size;				// 一次异步wsasend的最大长度
		DWORD				m_send_buffers;					// 等待发送的最大消息条数
		bool				m_minilzo;						// 是否使用lzo压缩算法发送数据
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SESSION_H_
