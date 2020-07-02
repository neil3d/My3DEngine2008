#pragma once

namespace nexus
{
	/** 
	* 消息监听器接口
	*/
	struct msg_listener
	{
		msg_listener() {};

		// 消息监听器
		// param message_id: 网络消息ＩＤ
		// param msg_ptr: 网络消息结构体指针
		virtual void on_msg(uint16 message_id, const void* msg_ptr) = 0;

		virtual void on_connected() = 0;

		virtual void on_disconnected() = 0;

		virtual void on_connect_timeout() = 0;

		virtual void on_connect_aborted() = 0;

		virtual void on_connect_error() = 0;

		DISALLOW_COPY_AND_ASSIGN(msg_listener);
	};

	typedef boost::shared_ptr<msg_listener> msg_listener_ptr;
}
