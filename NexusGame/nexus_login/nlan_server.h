/**
 *	nexus nlogin - nlan_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLAN_SERVER_H_
#define _NLOGIN_NLAN_SERVER_H_

#include "nnetwork.h"

namespace nexus {

	class nlan_session;

	/**
	 *	gateway, world的服务器
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

	private:
		nlan_session*				m_session_ptrs;						// session指针容器
		uint32						m_max_session;						// 容纳的最大session数目
	};

	extern nlan_server g_lan_server;

} // namespace nexus

#endif // _NLOGIN_NLAN_SERVER_H_
