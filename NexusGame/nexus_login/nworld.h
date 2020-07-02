/**
 *	nexus nlogin - nworld
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author:
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NWORLD_H_
#define _NLOGIN_NWORLD_H_

#include "ncommon.h"
#include "nlogin_utility.h"

namespace nexus {

	class nlogin_player;

	class nworld : private nnoncopyable
	{
	public:
		nworld();
		~nworld();

		bool init(uint32 world_session_id, uint32 world_id, const tstring& world_name);
		void destroy();
		void update(uint32 elapse);

		void set_gateway_session_id(uint32 gateway_session_id)	{ m_gateway_session_id = gateway_session_id; }

		uint32 get_world_id() const					{ return m_world_id; }
		uint32 get_world_name_crc() const			{ return m_world_name_crc; }
		const tstring& get_world_name() const		{ return m_world_name; }

		bool send_world_msg(void* msg_ptr, uint32 size);
		bool send_gateway_msg(void* msg_ptr, uint32 size);

		// 当前世界服务器是否连接良好
		bool is_well(void);

		// 加入等待验证玩家
		void add_proof_player(nlogin_player* player_ptr);

		// 移除验证列表中的玩家
		void remove_from_proof(nlogin_player* player_ptr);

		// 玩家进入排队
		void add_queue_player(nlogin_player* player_ptr);

		// 从排队队列中移除
		void remove_from_queue(nlogin_player* player_ptr);

		// 玩家退出
		void login_out(uint32 account_id);

		// 找出在验证队列中的玩家
		nlogin_player* get_proofing_player(uint64 client_id);

	private:

		void update_queue(uint32 elapse);

		// 当前在线人数是否达到上限
		bool is_upper(void);

	private:
		uint32			m_world_session_id;

		uint32			m_gateway_session_id;
		uint32			m_world_id;
		uint32			m_world_name_crc;
		tstring			m_world_name;

		PlayerList		m_proofing_playeres;	//验证的玩家列表
		PlayerList		m_queuing_playeres;		//排队的玩家列表

		uint32			m_max_player_num;		//最大可容纳人数
		uint32			m_cur_player_num;		//当前在线人数
		uint32			m_per_permit_num;		//每一个tick允许进入world的人数
	};

} // namespace nexus

#endif // _NLOGIN_NWORLD_H_
