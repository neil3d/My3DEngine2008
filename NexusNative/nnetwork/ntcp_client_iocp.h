/**
 *	nexus network - ntcp_client_iocp
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NTCP_CLIENT_IOCP_H_
#define _NNETWORK_NTCP_CLIENT_IOCP_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ntcp_session_client.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	ntcp_client_iocp 压力测试客户端通信底层
	 */
	class nNET_API ntcp_client_iocp : private nnoncopyable
	{
	private:
		friend class ntcp_session_client;

	public:
		ntcp_client_iocp();
		virtual ~ntcp_client_iocp();

		/** 
		 * 启动客户端，初始化资源
		 */
		bool startup(uint32 max_session = 1000, uint32 max_recv_buffer = 48 * 1024, uint32 max_send_buffer = 32, uint32 max_send_msg_size = 512, uint32 time_out_sec = 60, bool nagle = false);

		/** 
		 * 关闭客户端，该函数所在执行线程不被强制关闭，建议在主线程中关闭
		 */
		void shutdown();

		/** 
		 * 异步的连接服务器
		 * @param ip_addresss	远程ip地址
		 * @param port			远程端口
		 * @return				连接id,0XFFFFFFFF表示连接请求投递失败
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * 向指定连接发送数据，内部nmessage不共享，内部回收nmessage
		 * @param session_id	连接id
		 * @param msg_ptr		共享消息的地址
		 * @param size			发送数据的实际长度
		 */
		NINLINE bool send(uint32 session_id, nmessage* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_max_session)
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
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].close(session_id);
		}

		/** 
		 * 新连接建立回调函数, 注意线程安全
		 * note: 函数不要处理过多的逻辑，封装一个消息投递到统一的逻辑消息队列
		 * @param session_id	连接id
		 * @param param1		参数1见EConnectCode
		 * @param param2		参数2
		 */
		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2) {}

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
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return;
			}

			// 回调上层关闭函数
			on_disconnect(session_id);
			// 将session归还个连接管理池
			_return_session(&(m_sessions_ptr[session_id & 0X0000FFFF]));
		}

		// 取得一个可用的会话
		NINLINE ntcp_session_client* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			ntcp_session_client* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_connect();
			return temp_ptr;
		}
		// 归还一个关闭的会话
		NINLINE void _return_session(ntcp_session_client* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

	private:
		HANDLE						m_completion_port;			// 客户端完成端口

		nfast_mutex					m_mutex;					// 保护连接池线程安全及状态改变
		ntcp_session_client*		m_sessions_ptr;
		std::list<ntcp_session_client*>	m_available_sessions;

		boost::thread_group			m_threads;					// 线程池
		uint32						m_thread_num;				// 工作线程数目

		nsafe_mem_pool*				m_pools_ptr;				// 内存池
		TCPServerStatus				m_status;

		uint32						m_max_session;				// 最大连接数
		uint32						m_max_recv_buffer;			// recv的最大长度
		bool						m_nagle;					// 是否开启nagle算法
	};

} // namespace nexus

#endif _NNETWORK_NTCP_CLIENT_IOCP_H_
