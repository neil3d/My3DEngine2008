/**
 *	nexus network - ntcp_client 广域网客户端通讯底层
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_CLIENT_H_
#define _NNETWORK_NTCP_CLIENT_H_

#include "ncommon.h"
#include "nmutex.h"

#include <string>
#include <boost/thread/thread.hpp>

namespace nexus {

	/** 
	 * 客户端on_connect回调函数参数枚举 
	 */
	enum EClientConnect
	{
		ECS_Connected			= 0,		// 已经连接好
		ECS_ConnectTimeout		= 1,		// 连接超时
		ECS_ConnectAborted		= 2,		// 连接请求被取消
		ECS_Error				= 3,		// 发生错误
		ECS_Max,
	};

	/**
	 *	ntcp_server广域网客户端端通信底层
	 */
	class nNET_API ntcp_client : private nnoncopyable
	{
	private:
		enum EClientStatus
		{
			ECS_NONE			= 0,		// 空
			ECS_INIT			= 1,		// 开始初始化
			ECS_OPEN			= 2,		// 可以connect
			ECS_CONNECTED		= 3,		// 连接成功
			ECS_CLOSE			= 4,		// 关闭
			ECS_MAX,
		};

	public:
		ntcp_client();
		virtual ~ntcp_client();

		/** 
		 * 启动客户端，初始化资源
		 * @param time_out_sec	连接超时设置，单位s
		 * @param nagle			true打开Nagle算法，false关闭Nagle算法
		 */
		bool startup(uint32 time_out_sec = 20, bool nagle = true);

		/** 
		 * 关闭客户端，该函数所在执行线程不被强制关闭，建议在主线程中关闭
		 */
		void shutdown();

		/** 
		 * 异步连接服务器，连接状况通过回调函数on_connect返回
		 * @param ip_addresss	远程ip地址
		 * @param port			远程端口
		 * @return				true异步连接请求投递成功，false投递失败
		 */
		bool connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * 关闭与服务器的链接，关闭成功后回调on_disconnect函数
		 */
		void close();

		/** 
		 * 向服务器发送数据，内部会建立一份拷贝，此函数返回后msg指向的内存可以安全释放
		 * note: 加密的功能在派生类封装
		 * @param msg_ptr	数据的首地址
		 * @param size		数据的长度
		 */
		bool send(const void* msg_ptr, uint32 size);

		/** 
		 * 客户端网络连接状态改变回调函数, 注意线程安全
		 * note: 函数不要处理过多的逻辑，建议只封装一个消息投递到统一的逻辑消息队列
		 * @param error_code	参数含义见EClientConnect
		 */
		virtual void on_connect(uint32 error_code) {}

		/** 
		 * 客户端接收网络消息回调函数，外部需要建立一份拷贝，不必考虑msg_ptr的释放, 注意线程安全
		 * note: 函数不要处理过多的逻辑，建议只封装一个消息投递到统一的逻辑消息队列
		 * @param msg_ptr	数据的首地址
		 * @param size		数据的长度
		 */
		virtual void on_datarecv(const void* msg_ptr, uint32 size) {}

		/** 
		 * 客户连接关闭回调函数，注意线程安全
		 * note: 函数不要处理过多的逻辑，封装一个消息投递到统一的逻辑线程队列
		 */
		virtual void on_disconnect() {}

	private:
		// 释放内存资源
		void _free_memory();
		// 完美关闭socket
		bool _graceful_close_socket(SOCKET& socket, int retry_times);
		// 工作线程，用于实现异步连接，异步关闭
		void _thread_worker();
		// 数据发送线程
		void _thread_send();
		// 数据接收线程
		void _thread_recv();

		// 处理connect
		void _handle_connect();
		// 处理close
		void _handle_close();

	private:
		EClientStatus				m_status;					// client状态
		SOCKET						m_socket;					// socket
		std::string					m_ip_addresss;				// 远程ip地址
		uint16						m_port;						// 远程端口
		uint32						m_time_out;					// 超时设置，单位s
		bool						m_nagle;					// 是否开启nagle算法

		boost::thread_group			m_threads;					// 线程池，读写线程
		boost::thread_group			m_thread_worker;			// 工作线程池，目前只用一个

		char*						m_send_buffer_ptr[2];		// 发送缓冲区
		DWORD						m_write_wr[2];				// 发送缓冲区写指针

		nmutex						m_send_mutex;				// 发送缓冲区安全锁，及控制client状态改变
		DWORD						m_send_index;				// 当前send的缓冲区索引

		char*						m_recv_buffer_ptr;			// 接收缓冲区

		volatile LONG				m_terminate;				// 1 表示强制退出读写线程, 0表示不退出
		volatile LONG				m_terminate_workder;		// 1 表示强制退出工作线程, 0表示不退出

		volatile LONG				m_connecting;				// 1 表示连接请求, 0表示不是
		volatile LONG				m_closing;					// 1 表示关闭请求, 0表示不是

		HANDLE						m_send_event;				// 发送事件
	};

} // namespace nexus

#endif _NNETWORK_NTCP_CLIENT_H_
