/**
 *	nexus network - ntcp_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_SERVER_H_
#define _NNETWORK_NTCP_SERVER_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ntcp_session.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	ntcp_server广域网服务器端通信底层
	 */
	class nNET_API ntcp_server : private nnoncopyable
	{
	private:
		friend class ntcp_session;

	public:
		ntcp_server();
		virtual ~ntcp_server();

		/** 
		 * 启动服务器监听指定端口的任意地址
		 */
		bool startup(const ntcp_server_config& param);

		/** 
		 * 关闭服务器，该函数所在执行线程不被强制关闭，建议在主线程中关闭
		 */
		void shutdown();

		/** 
		 * 向指定连接发送数据，内部采用引用计数的方式访问数据
		 * 如果同一nmessage被send多次要先置所有的引用次数再调用send, server内部会释放nmessage内存数据
		 * @param session_id	连接id
		 * @param msg_ptr		共享消息的地址
		 * @param size			发送数据的实际长度
		 */
		NINLINE bool send(uint32 session_id, nmessage* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				msg_ptr->dec_reference();
				return false;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].send(session_id, msg_ptr, size);
		}

		/** 
		 * 关闭指定连接
		 * @param session_id	连接id
		 */
		NINLINE void close(uint32 session_id)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				return;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].close(session_id);
		}

		/** 
		 * 新连接建立回调函数, 注意线程安全
		 * note: 函数不要处理过多的逻辑，封装一个消息投递到统一的逻辑消息队列
		 * @param session_id	连接id
		 * @param address		连接ip地址
		 * @param port			连接端口
		 */
		virtual void on_connect(uint32 session_id, uint32 address, uint32 port) {}

		/** 
		 * 消息接收回调函数, 注意线程安全
		 * note: recv nmessage 的引用计数都为0，调用回调后网络底层不在引用该内存
		 * 函数不要处理过多的逻辑，转发消息或将消息投递到统一的逻辑消息队列
		 * @param session_id	连接id
		 * @param msg_ptr		消息地址
		 * @param port			消息的实际长度
		 */
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size) {}

		/** 
		 * 连接关闭回调函数, 注意线程安全
		 * note: 函数不要处理过多的逻辑，封装一个消息投递到统一的逻辑线程队列
		 * @param session_id	连接id
		 */
		virtual void on_disconnect(uint32 session_id) {}

	private:
		// 释放该类申请的内存资源
		void _free_memory();
		// iocp工作线程负责处理完成包
		void _worker_thread(int32 thread_index);
		// 异步accpet请求
		void _async_accept();
		// 异步accpet请求响应回调
		void _handle_accept(ntcp_session* session_ptr, DWORD last_error, nsafe_mem_pool* pool_ptr);
		// 向完成端口投递一个close回调请求
		NINLINE void _post_close_call_back(uint32 session_id)
		{
			if (0XFFFFFFFF == session_id)
			{
				return;
			}

			::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)session_id, 0);
		}

		NINLINE void _handle_close(uint32 session_id)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				return;
			}

			// 回调上层关闭函数
			on_disconnect(session_id);
			// 将session归还个连接管理池
			_return_session(&(m_sessions_ptr[session_id & 0X0000FFFF]));
			_async_accept();
		}

		// 取得一个可用的会话
		NINLINE ntcp_session* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			ntcp_session* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_accept();
			return temp_ptr;
		}
		// 归还一个关闭的会话
		NINLINE void _return_session(ntcp_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

	private:
		SOCKET						m_sock_listen;				// 监听socket
		HANDLE						m_completion_port;			// 服务器完成端口

		LPFN_ACCEPTEX				m_fn_acceptex;
		LPFN_GETACCEPTEXSOCKADDRS	m_fn_get_acceptex_sock_addr;
		volatile LONG				m_acceptexs;				// 放出的AcceptEx数量

		nfast_mutex					m_mutex;					// 保护连接池线程安全及server状态改变
		ntcp_session*				m_sessions_ptr;
		std::list<ntcp_session*>	m_available_sessions;

		boost::thread_group			m_threads;					// 线程池

		ntcp_server_config			m_ntcp_server_param;
		LONG						m_max_acceptex;

		nsafe_mem_pool*				m_pools_ptr;				// 内存池

		TCPServerStatus				m_status;
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SERVER_H_
