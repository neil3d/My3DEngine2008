/**
 *	nexus network - nnetwork_def.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NNETWORK_DEF_H_
#define _NNETWORK_NNETWORK_DEF_H_

#include "ncommon.h"
#include "nsafe_mem_pool.h"

namespace nexus {

	class ntcp_session;

#pragma pack(push,1)

	/**
	 *	IOCP完成包类型
	 */
	enum IOCompletionStatus
	{
		IOCS_ACCEPT				= 0,		// 接受请求完成或连接请求完成
		IOCS_READ				= 1,		// 读请求完成
		IOCS_WRITE				= 2,		// 写请求完成
		IOCS_MAX				= 3,
	};

	/**
	 *	网络底层session状态, ntcp_session, ncluster_session, ntcp_session_client
	 */
	enum TCPSessionStatus
	{
		TSS_NONE				= 0,
		TSS_INIT				= 1,
		TSS_ACCEPTEX			= 2,
		TSS_OPEN				= 3,
		TSS_SHUT				= 4,
		TSS_CLOSE				= 5,
		TSS_MAX					= 6,
	};

	/**
	 *	网络底层状态, ntcp_server, ncluster_server, ncluster_client, ntcp_client_iocp
	 */
	enum TCPServerStatus
	{
		TSVRS_NONE				= 0,
		TSVRS_INIT				= 1,
		TSVRS_OPEN				= 3,
		TSVRS_SHUT				= 4,
		TSVRS_CLOSE				= 5,
		TSVRS_MAX				= 6,
	};

	/**
	 *	on_connect回调函数参数枚举值, ncluster_client, ntcp_client_iocp
	 */
	enum EConnectCode
	{
		ECC_Connected			= 0,		// 已经连接好
		ECC_ConnectTimeout		= 1,		// 连接超时
		ECC_ConnectAborted		= 2,		// 连接请求被取消
		ECC_Error				= 3,		// 连接时发生错误
		ECC_Max,
	};

	struct noverlappedex
	{
		OVERLAPPED			ov;
		IOCompletionStatus	status;

		noverlappedex() : status(IOCS_MAX)
		{
			ov.Internal		= 0;
			ov.InternalHigh	= 0;
			ov.Offset		= 0;
			ov.OffsetHigh	= 0;
			ov.hEvent		= 0;
		}
	};

	struct noverlappedex2 : public noverlappedex
	{
		void*		session_ptr;

		noverlappedex2() : session_ptr(NULL) {}
	};

	struct ntcp_server_config
	{
		uint16			port;						// 端口, 绑定所有地址
		uint32			max_session;				// 服务器承载最大连接数
		uint32			max_send_size;				// 单次发送的最大字节数(压缩前),不要超过客户端的缓冲区大小48 * 1024!!!
		uint32			max_send_msg;				// WSASend发送最大消息数
		uint32			max_recv_size;				// 最大接受包长度
		bool			reuse_port;					// 是否重用端口
		bool			nagle;						// 是否使用Nagle算法
		bool			minilzo;					// 是否使用minilzo算法

		ntcp_server_config() : port(6000), max_session(4000), max_send_size(48 * 1024), max_send_msg(200), max_recv_size(512), 
			reuse_port(false), nagle(false), minilzo(true) {}
	};

	struct nsend_buffer
	{
		void*			buffer_ptr;
		nsend_buffer*	next_ptr;
		uint32			size;

		nsend_buffer() : buffer_ptr(NULL), next_ptr(NULL), size(0) {}
	};

	// 该结构体用于共享，注意哪些是不变的，哪些是为了共享可以变的
	struct nmessage
	{
		nsafe_mem_pool*	pool_ptr;					// 记录从哪个mem_pool分配
		nmessage*		next_ptr;					// 指向下一个消息, 用于构建逻辑处理消息队列
		volatile LONG	reference;					// 记录的引用计数
		uint32			size;						// 接收的有效内容长度, 外网接收首地址&buffer, 内网接收首地址&session_id
		uint32			session_id;					// 服务器消息首地址&session_id
		uint32			server_id;					// ngateway服务器id，由nworld分配
		char			buffer[sizeof(uint32)];		// 外网接收首地址&buffer

		nmessage() : pool_ptr(NULL), next_ptr(NULL), reference(0), size(0), session_id(0), server_id(0) {}

		// 用于释放send时的引用资源, 逻辑消息的处理后直接释放，如果pool为null crush
		void dec_reference()
		{
			if (0 == ::InterlockedDecrement(&reference))
			{
				pool_ptr->free(this);
			}
		}

		// 归还所属内存池，如果pool为null crush
		void free()
		{
			pool_ptr->free(this);
		}
	};

	#define RECV_SIZE(size) (sizeof(nmessage) + (size))

	#define	NSMSG_OFFSET		(sizeof(nsafe_mem_pool*) + sizeof(LONG) + sizeof(nmessage*) + sizeof(uint32))
	#define	NCMSG_OFFSET		(NSMSG_OFFSET + sizeof(uint32) + sizeof(uint32))
	#define NC2MSG(base_ptr)	(reinterpret_cast<nmessage*>(reinterpret_cast<char*>(base_ptr) - NCMSG_OFFSET))
	#define NS2MSG(base_ptr)	(reinterpret_cast<nmessage*>(reinterpret_cast<char*>(base_ptr) - NSMSG_OFFSET))

#pragma pack(pop)

} // namespace nexus

#endif _NNETWORK_NNETWORK_DEF_H_
