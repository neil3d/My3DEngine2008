#pragma once
#include "nnetwork.h"
#include "msg_queue.h"
#include "msg_listener.h"

namespace nexus
{
	enum EClientSessionStatus
	{
		ECSS_None				= 0,
		ECSS_Connected			= 1,
		ECSS_Disconnect			= (1<<5),
	};


	struct client_msg;
	struct msg_listener;

	/**
	*	客户端数据传输类
	*/
	class client_connection 
		: public nexus::ntcp_client/*, public singleton<client_connection>*/
	{
	public:
		client_connection();
		virtual ~client_connection();

		// 网络层收到的消息都缓存在消息队列中
		// 只有在Update的时候才从消息队列中取出真正处理消息
		void update(float delta_time);

		// 添加消息监听器
		void add_listener(msg_listener_ptr listener)
		{
			if (!listener)
				return;

			m_msg_listeners.push_back( listener );
		}

		// 移除消息监听器
		void remove_listener(msg_listener_ptr listener)
		{
			if (!listener)
				return;

			if (m_msg_listeners.empty())
				return;
			m_msg_listeners.erase( std::find(m_msg_listeners.begin(), m_msg_listeners.end(), listener) );
		}

		void clear_listeners()
		{
			m_msg_listeners.clear();
		}

		virtual void on_connect(uint32 error_code);
		virtual void on_datarecv(const void* msg_ptr, uint32 size);
		virtual void on_disconnect();

		// Session状态
		void set_status(unsigned int  status)	{ m_status = status; }
		unsigned int get_status() const		{ return m_status; }

	private:
		// 客户端真正解析和处理消息
		void on_handle_msg(client_msg* msg_ptr);

		// on_datarecv时会调用
		bool push_msg(const void* msg_ptr, uint32 size)
		{
			return m_msg_queue.enqueue(msg_ptr, size);
		}


	private:
		// Session状态
		unsigned int 				m_status;

		// 消息发送队列
		msg_queue					m_msg_queue;
	
		// 消息监听器,外部管理其生存周期
		typedef std::vector<msg_listener_ptr> msg_listener_list;
		msg_listener_list			m_msg_listeners;
	};

}
