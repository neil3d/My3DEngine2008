/**
 *	nexus nlogin - nlan_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLAN_SESSION_H_
#define _NLOGIN_NLAN_SESSION_H_

#include "ncommon.h"
#include "nmsg_queue.h"

namespace nexus {

	struct nmessage;
	struct nmsg_base;

	class nlan_session : private nnoncopyable
	{
	public:
		nlan_session();
		virtual ~nlan_session();

		bool init(uint32 session_id, const std::string& address, uint16 port);
		void destroy();
		void update(uint32 elapse);

		bool push_msg(nmessage* msg_ptr)			{ return m_msg_queue.enqueue(msg_ptr); }
		void set_status(uint32 status)				{ m_status = status; }
		void set_sesssion_id(uint32 session_id)		{ m_session_id = session_id; }
		void set_server_id(uint32 server_id) 		{ m_server_id = server_id; }

		bool   is_in_used() const					{ return 0XFFFFFFFF != m_session_id; }
		uint32 get_status() const					{ return m_status; }
		uint32 get_type() const						{ return m_server_type; }
		uint32 get_sesssion_id() const				{ return m_session_id; }
		uint32 get_server_id() const				{ return m_server_id; }
		uint32 get_world_id() const					{ return m_server_id & 0X00FFFFFF; }

		bool send_message(const void* msg_ptr, uint32 size);

	public:
		// handler
		void handle_world_request_login(const nmsg_base* packet);
		void handle_request_login(const nmsg_base* packet);
		void handle_login_proof(const nmsg_base* packet);
		void handle_account_logout(const nmsg_base* packet);

		// 角色登录账号验证(不需要验证)
		void handle_login_proof_1(const nmsg_base* packet);
		// 角色登录账号验证(需要验证账号、密码、密保)
		void handle_login_proof_2(const nmsg_base* packet);
		// 角色登录密保验证
		void handle_mibao(const nmsg_base* packet);

	private:
		void _handle_msg();

	private:
		uint32			m_session_id;
		uint32			m_server_type;
		uint32			m_server_id;
		uint32			m_status;
		std::string		m_address;
		uint16			m_port;

		nmsg_queue		m_msg_queue;
	};

} // namespace nexus

#endif // _NLOGIN_NLAN_SESSION_H_
