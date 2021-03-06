/**
 *	nexus network - ncluster_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NCLUSTER_SERVER_H_
#define _NNETWORK_NCLUSTER_SERVER_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ncluster_session.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	ncluster_server局域网服务器端传输底层
	 *	@remark ncluster_client  服务器组内通信服务器端，为每个连接开辟读写线程，适用于大流量少连接通讯
	 */
	class nNET_API ncluster_server : private nnoncopyable
	{
	private:
		friend class ncluster_session;

	public:
		ncluster_server();
		virtual ~ncluster_server();

		/** 
		 * 启动服务器监听指定端口的任意地址
		 */
		bool startup(uint16 port, uint32 max_session = 4, uint32 max_send_buffer = 2 * 1024 * 1024, bool reuse_port = false, bool nagle = true);

		/** 
		 * 关闭服务器，该函数所在执行线程不被强制关闭，建议在主线程中关闭
		 */
		void shutdown();

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
		void close(uint32 session_id)
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
		 * @param param1		参数1
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
		// 实现异步accept线程
		void _thread_accept();
		// 连接关闭线程
		void _thread_close();

		// 取得一个可用的会话
		NINLINE ncluster_session* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			ncluster_session* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_accept();
			return temp_ptr;
		}
		// 归还一个关闭的会话
		NINLINE void _return_session(ncluster_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

		// 取得一个close请求会话
		NINLINE ncluster_session* _get_closing_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (m_closing_sessions.empty())
			{
				return NULL;
			}

			ncluster_session* temp_ptr = m_closing_sessions.front();
			m_closing_sessions.pop_front();
			return temp_ptr;
		}

		// 投递一个close请求
		NINLINE void _closing_session(ncluster_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_closing_sessions.push_back(session_ptr);
		}

	private:
		TCPServerStatus				m_status;
		SOCKET						m_sock_listen;				// 监听socket

		nfast_mutex					m_mutex;					// 保护连接池线程安全及server状态改变，可用和关闭公用一个锁，cluster基本不频繁断开重连
		ncluster_session*			m_sessions_ptr;				// 连接池
		std::list<ncluster_session*>m_available_sessions;		// 可用连接集合
		std::list<ncluster_session*>m_closing_sessions;

		boost::thread_group			m_threads;					// 线程池

		volatile LONG				m_terminate_accept;			// 1 关闭accept线程，0线程正常运行
		volatile LONG				m_terminate_close;			// 1 关闭close线程常运行

		sockaddr_in					m_address;					// server address
		uint16						m_port;						// 端口
		uint32						m_max_session;				// 最大连接数
		bool						m_reuse_port;				// 是否重用端口
		bool						m_nagle;					// 是否开启nagle算法
	};

} // namespace nexus

#endif _NNETWORK_NCLUSTER_SERVER_H_
