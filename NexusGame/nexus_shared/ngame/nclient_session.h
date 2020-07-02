/**
 *	nexus ngame - nclient_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NCLIENT_SESSION_H_
#define _NGAME_NCLIENT_SESSION_H_

#include "ncommon.h"
#include "nmsg_buff.h"

namespace nexus {

	class nplayer;

	enum ESessionStatus
	{
		ESS_None				= 0X0000,
		ESS_Logined				= 0X0001,
		ESS_DBLoading			= 0X0002,
		ESS_Loading				= 0X0004,
		ESS_Loaded				= 0X0008,
		ESS_Gaming				= 0X0010,
		ESS_DBSaving			= 0X0020,
		ESS_DBSaved				= 0X0040,
		ESS_Max
	};

	class nclient_session : private nnoncopyable
	{
	public:
		nclient_session();
		virtual ~nclient_session();

		bool init(uint64 client_id, uint32 gateway_session_id, uint32 account_id, uint8 privilege, uint16 port, uint32 ip_address, const nchar* account, uint64 logout_time);
		void destroy();
		void update(uint32 elapse, bool world_update);

		uint64 get_client_id() const { return m_client_id; }
		uint32	get_lan_session_id() const	{ return m_gateway_session_id; }
		uint32	get_wan_session_id() const	{ return (uint32)(m_client_id & 0x00000000FFFFFFFFui64); }
		tstring	get_account() const		{ return m_account; }
		uint8	get_privilege() const		{ return m_privilege; }
		nplayer*get_player() const			{ return m_player_ptr; }
		uint8	get_status() const			{ return m_status; }

		void	set_status(uint8 status)	{ m_status = status; }

		nmessage* push_msg(nmessage* msg_ptr, bool map_task, bool& overflow) {	return m_msg_buff.push_msg(msg_ptr, map_task, overflow); }

		// 发送到客户端
		bool send_message(void* msg_ptr, uint32 size);

		// 发送到客户端
		bool send_message_to_db(void* msg_ptr, uint32 size);

	public:
		// handler
		// character manager
		void handle_char_enum(const nmsg_base* msg_ptr);
		void handle_char_create(const nmsg_base* msg_ptr);
		void handle_char_delete(const nmsg_base* msg_ptr);
		void handle_char_enter_game(const nmsg_base* msg_ptr);

		// loading
		void handle_get_init_stat(const nmsg_base* msg_ptr);
		void handle_get_init_spell(const nmsg_base* msg_ptr);
		void handle_get_init_aura(const nmsg_base* msg_ptr);

		// map
		void handle_map_enter(const nmsg_base* msg_ptr);
		void handle_map_teleport_ack(const nmsg_base* msg_ptr);


		// movement
		void handle_player_move(const nmsg_base* msg_ptr);
		void handle_check_time(const nmsg_base* msg_ptr);

		// chat
		void handle_player_chat(const nmsg_base* msg_ptr);

		// spell
		void handle_cast_spell(const nmsg_base* msg_ptr);
		void handle_cancel_cast(const nmsg_base* msg_ptr);
		void handle_cancel_aura(const nmsg_base* msg_ptr);

	private:
		nmsg_buff			m_msg_buff;					// 消息缓冲控制
		uint64				m_client_id;				// session的唯一id, 标识属于哪个gateway, gateway中哪个session, 用于与客户端通信
		uint32				m_gateway_session_id;		// client所在gateway在world中的sessionid, BG需要客户端确认设置
		uint32				m_account_id;				// 帐号ID, 也可以标识唯一session
		uint8				m_status;					// session状态
		uint8				m_privilege;				// 账号权限
		uint16				m_port;						// 客户端端口
		uint32				m_ip_address;				// 客户端ip地址
		tstring				m_account;					// 玩家账号
		time_t				m_logout_time;				// 上次logout时间
		time_t				m_login_time;				// 本次login时间
		nplayer*			m_player_ptr;				// session一一对应的角色对象
	};

} // namespace nexus

#endif // _NGAME_NCLIENT_SESSION_H_
