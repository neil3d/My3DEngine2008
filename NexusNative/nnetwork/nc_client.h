/**
 *	nexus network - nc_client
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NNETWORK_NC_CLIENT_H_
#define _NNETWORK_NC_CLIENT_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "nc_session.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	nc_client 局域网客户端端通信底层
	 */
	class nNET_API nc_client : private nnoncopyable
	{
	private:
		friend class nc_session;

	public:
		nc_client();
		virtual ~nc_client();

		/** 
		 * 启动客户端，初始化资源
		 */
		bool startup(uint32 max_session = 4, uint32 thread_num = 4, uint32 max_buffer_size = 2 * 1024 * 1024, bool nagle = true);

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
		 * 向指定连接发送数据，内部采用引用计数的方式访问数据
		 * 如果同一nmessage被send多次要先置所有的引用次数再调用send, server内部会释放nmessage内存数据
		 * note: gateway使用接口
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
		 * 向指定连接发送数据，内部会建立一份拷贝，此函数返回后msg指向的内存可以安全释放
		 * note: 游戏逻辑服务器使用接口
		 * @param session_id	连接id
		 * @param msg_ptr		消息的地址
		 * @param size			发送数据的实际长度
		 */
		NINLINE bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return false;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].send_msg(session_id, msg_ptr, size);
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
		NINLINE nc_session* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			nc_session* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_accept_or_connect();
			return temp_ptr;
		}
		// 归还一个关闭的会话
		NINLINE void _return_session(nc_session* session_ptr)
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
		TCPServerStatus				m_status;

		nfast_mutex					m_mutex;					// 保护连接池线程安全及状态改变
		nc_session*					m_sessions_ptr;
		std::list<nc_session*>		m_available_sessions;

		boost::thread_group			m_threads;					// 线程池
		uint32						m_thread_num;				// 工作线程数目
		
		uint32						m_max_session;				// 最大连接数
		bool						m_nagle;					// 是否开启nagle算法

		nsafe_mem_pool*				m_pools_ptr;				// 内存池
	};

} // namespace nexus

#endif // _NNETWORK_NC_CLIENT_H_
