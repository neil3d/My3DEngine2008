/**
 *	nexus network - ncluster_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NCLUSTER_SESSION_H_
#define _NNETWORK_NCLUSTER_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	class ncluster_server;
	class ncluster_client;

	/**
	 *	ncluster_session局域网网络传输会话
	 */
	class nNET_API ncluster_session : private nnoncopyable
	{
	private:
		friend class ncluster_server;
		friend class ncluster_client;

	public:
		ncluster_session();
		virtual ~ncluster_session();

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

			_closing();
		}

	private:
		// 连接初始化，设置参数，申请资源, 服务器和客户端公用该session通过传递的参数区分server_ptr和client_ptr有且只有一个非空
		bool _init(uint16 index, ncluster_server* server_ptr, ncluster_client* client_ptr, uint32 max_send_buffer);
		// 释放连接申请的内存资源，不包括pool申请资源，pool资源在close时归还
		void _free_memory();
		// 连接在投递acceptex前或connect前数据初始化
		void _pre_accept();
		// 连接打开回调函数，创建读写线程
		void _open();
		// 归还各种pool资源
		void _release();
		// 关闭请求
		void _closing();
		// 关闭指定连接
		void _close();

		// 设置session状态
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// 获得session状态
		NINLINE TCPSessionStatus _get_status() const { return m_status; }
		// 获得session id
		NINLINE uint32 _get_client_id() const { return m_session_id; }

		// 读数据线程
		void _thread_recv();
		// 写数据线程
		void _thread_send();

	private:
		uint32				m_session_id;					// session_id一个连接| 低2字节 表示index | 高2字节表示guid |，0XFFFFFFFF为非法的client_id
		ncluster_server*	m_server_ptr;					// 上层server指针，用于回调消息
		ncluster_client*	m_client_ptr;					// 上层client指针，用于回调消息
		SOCKET				m_socket;						// session的socket资源
		sockaddr_in			m_address;						// 该连接connect的服务器地址

		TCPSessionStatus	m_status;						// session状态

		nfast_mutex				m_mutex;						// 保护send缓冲区和session状态
		uint32					m_write_wr[2];					// 发送缓冲区写指针
		char*					m_buffer[2];					// 发送缓冲区
		uint32					m_send_index;					// 当前send的缓冲区索引
		uint32					m_max_send_buffer;				// 发送缓冲区的最大长度
		HANDLE				m_send_event;					// 发送事件

		volatile LONG			m_terminate;					// 1 关闭读写线程
		nsafe_mem_pool		m_pool;							// recv函数回调申请内存
		boost::thread_group	m_threads;
	};

} // namespace nexus

#endif _NNETWORK_NCLUSTER_SESSION_H_
