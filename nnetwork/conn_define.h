#ifndef _NEXUS_SERVER_CALL_BACK_H_
#define _NEXUS_SERVER_CALL_BACK_H_
#include <boost/function.hpp>
#include "net_packet.h"

namespace nexus
{
	/** 网络连接状态枚举 */
	enum EConnectionState
	{
		ECS_Invalid,		// 无效状态
		ECS_Connecting,		// 正在连接中
		ECS_Connected,		// 已经连接好
		ECS_ConnectTimeout,	// 连接超时
		ECS_Disconnected,	// 已经断开连接
		ECS_Error,			// 发生错误
	};

	typedef boost::function<void (enum EConnectionState)> conn_state_call_back;

	// tcp connection
	typedef boost::function<void (nnet_packet_ptr)>	recv_handler;	
}//namespace nexus

#endif // _NEXUS_SERVER_CALL_BACK_H_