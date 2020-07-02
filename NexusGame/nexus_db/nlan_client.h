/**
 *	nexus ndb - nlan_client
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDB_NLAN_CLIENT_H_
#define _NDB_NLAN_CLIENT_H_

#include "nnetwork.h"

namespace nexus {

	class nlan_session;

	/**
	 *	world的客户端
	 */
	class nlan_client : public nc_client
	{
	public:
		nlan_client();
		virtual ~nlan_client();

		bool init(uint32 max_session);
		void destroy();
		void update(uint32 elapse);

		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

		nlan_session* get_session(uint32 session_id);

		bool connect_to_nworld();

		bool send_nworld(const void* msg_ptr, uint32 size);

	private:
		bool _register_session(uint32 session_id, uint32 server_type, const std::string& address, uint16 port);
		void _unregister_session(uint32 session_id);

		void _handle_connect_nworld(uint32 param);
		void _handle_close_nworld();

	private:
		nlan_session*			m_session_ptrs;					// session指针容器
		uint32					m_max_session;					// 容纳的最大session数目

		uint32					m_nworld_session_id;
		int32					m_retry_nworld_elapse;
	};

	extern nlan_client g_lan_client;

} // namespace nexus

#endif // _NDB_NLAN_CLIENT_H_
