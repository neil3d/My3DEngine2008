/**
 *	nexus ngame - nlan_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NLAN_SERVER_H_
#define _NGAME_NLAN_SERVER_H_

#include "nnetwork.h"

namespace nexus {

	class nlan_session;

	/**
	 *	gateway, db, 的服务器(world), or gateway, world, 的服务器(bg)
	 */
	class nlan_server : public nc_server
	{
	public:
		nlan_server();
		virtual ~nlan_server();

		bool init(uint32 max_session);
		void destroy();
		void update(uint32 elapse);

		// 网络层回调函数
		virtual void on_connect(uint32 session_id, uint32 address, uint32 port);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

		nlan_session* get_session(uint32 session_id);

		void set_db_session_id(uint32 session_id) { m_db_sesssion_id = session_id; }
		uint32 get_db_session_id() const { return m_db_sesssion_id; }

		bool send_ndb(const void* msg_ptr, uint32 size);
		void broadcast(const void* msg_ptr, uint32 size);				// 全服广播

	private:
		nlan_session*				m_session_ptrs;						// session指针容器
		uint32						m_max_session;						// 容纳的最大session数目
		uint32						m_db_sesssion_id;					// 只有一个服务器, gateway world的话可能有多个，在lan_session中处理直接回复, player seesion 存储相应id
	};

	extern nlan_server g_lan_server;

} // namespace nexus

#endif // _NGAME_NLAN_SERVER_H_
