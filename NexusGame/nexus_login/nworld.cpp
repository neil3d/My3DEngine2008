#include "ncfg.h"
#include "nworld.h"
#include "nlan_server.h"
#include "nlog_mt.h"
#include "nlogin_db.h"
#include "nlogin_player.h"
#include "nplayer_account_mgr.h"
#include "nmsg_world_login.h"

#include "nenums.h"
#include "nlan_session.h"

#include <boost/crc.hpp>

namespace nexus {


	nworld::nworld() : m_world_session_id(0), m_gateway_session_id(0), m_world_id(0), m_world_name_crc(0),
		m_max_player_num(1000), m_cur_player_num(0),m_per_permit_num(2)
	{
		// to do(by leo) : 以后通过config来设定 m_max_player_num、m_per_permit_num
		m_max_player_num	= sncfg.get_int32(ECI_MaxPlayerNum);
		m_per_permit_num	= sncfg.get_int32(ECI_PerPermitPlayer);
	}

	nworld::~nworld()
	{

	}

	bool nworld::init(uint32 world_session_id, uint32 world_id, const tstring& world_name)
	{
		m_world_session_id	= world_session_id;
		m_world_id			= world_id;
		m_world_name		= world_name;

		boost::crc_32_type  crc_result;
		crc_result.process_bytes(m_world_name.data(), m_world_name.size() * sizeof(tstring::value_type));

		m_world_name_crc	= crc_result.checksum();
		return true;
	}

	void nworld::destroy()
	{

	}

	void nworld::update(uint32 elapse)
	{
		update_queue(elapse);
	}

	bool nworld::send_world_msg(void* msg_ptr, uint32 size)
	{
		return g_lan_server.send_msg(m_world_session_id, msg_ptr, size);
	}

	bool nworld::send_gateway_msg(void* msg_ptr, uint32 size)
	{
		return g_lan_server.send_msg(m_gateway_session_id, msg_ptr, size);
	}

	// 当前世界服务器是否连接良好
	bool nworld::is_well(void)
	{
		if( g_lan_server.get_session(m_world_session_id) )
			return ( ELSS_Logined == g_lan_server.get_session(m_world_session_id)->get_status() );

		return false;
	}

	// 加入等待验证玩家
	void nworld::add_proof_player(nlogin_player* player_ptr)
	{
		if(!player_ptr) return;

		player_ptr->set_world_crc(m_world_name_crc);
		player_ptr->set_status(EPLS_Proofing);
		m_proofing_playeres.push_back(player_ptr);

		return;
	}

	// 玩家进入排队
	void nworld::add_queue_player(nlogin_player* player_ptr)
	{
		if(!player_ptr) return ;

		if( m_world_name_crc != player_ptr->get_world_crc() )
			return;

		if( player_ptr->is_status(EPLS_Queuing) )
			return;

		if( !player_ptr->is_status(EPLS_End_MiBao) )
			return;

		// 进入排队队列
		m_queuing_playeres.push_back(player_ptr);
		player_ptr->set_status(EPLS_Queuing);

		// 从验证队列移除
		remove_from_proof(player_ptr);
	}

	void nworld::update_queue(uint32 elapse)
	{
		if( !m_queuing_playeres.size() )
			return;

#ifdef	NWORLD_UPDATE_QUEUE
		float	elapse_time = get_time();
#endif

		uint32 cur_enter_world_num = 0;
		for( ; m_queuing_playeres.size();  )
		{
			if( is_upper() ) break;

			nlogin_player* player_ptr = m_queuing_playeres.front();

			tagMsg_L2W_Player_Login request;
			request.client_id		= player_ptr->get_client_id();
			request.account_id		= player_ptr->get_account_id();
			request.online_time		= 100;
			request.privilege		= player_ptr->get_privilege();

			send_world_msg(&request, sizeof(request));

			player_ptr->set_status(EPLS_EnterWorld);
			nlogin_player::free_login_player(player_ptr);
			m_queuing_playeres.pop_front();


			++m_cur_player_num;
			++cur_enter_world_num;
			if( cur_enter_world_num>=m_per_permit_num )
			{
#ifdef	NWORLD_UPDATE_QUEUE
				elapse_time = get_time() - elapse_time;
				NLOG_INFO(_T("%s, num is %d, time is %f"), __TFUNCTION__, cur_enter_world_num, elapse_time);
#endif
				break;
		}

#ifdef	NWORLD_UPDATE_QUEUE
			if( !m_queuing_playeres.size() )
			{
				elapse_time = get_time() - elapse_time;
				NLOG_INFO(_T("%s, num is %d, time is %f"), __TFUNCTION__, cur_enter_world_num, elapse_time);
	}
#endif
	
	}
	}

	// 当前在线人数是否达到上限
	bool nworld::is_upper(void)
	{
		return ( m_cur_player_num >= m_max_player_num );
	}

	void nworld::remove_from_proof(nlogin_player* player_ptr)
	{
		if( !player_ptr ) return;

		PlayerListIt it = std::find(m_proofing_playeres.begin(), m_proofing_playeres.end(), player_ptr);
		if( m_proofing_playeres.end() != it)
			m_proofing_playeres.erase( it );
	}

	void nworld::remove_from_queue(nlogin_player* player_ptr)
	{
		if( !player_ptr ) return;

		PlayerListIt it = std::find(m_queuing_playeres.begin(), m_queuing_playeres.end(), player_ptr);
		if( m_queuing_playeres.end() != it)
			m_queuing_playeres.erase( it );
	}

	// 玩家退出
	void nworld::login_out(uint32 account_id)
	{
		for( PlayerListIt it=m_proofing_playeres.begin(); m_proofing_playeres.end()!=it; ++it )
		{//玩家在验证
			if( (*it)->get_account_id() != account_id )
				continue;

			(*it)->set_status(EPLS_None);
			g_login_db.login_out((*it)->get_account_id(), (*it)->get_cur_ip());
			nlogin_player::free_login_player(*it);
			m_proofing_playeres.erase(it);
			return;
		}

		for( PlayerListIt it=m_queuing_playeres.begin(); m_queuing_playeres.end()!=it; ++it )
		{//玩家在排队
			if( (*it)->get_account_id() != account_id )
				continue;

			(*it)->set_status(EPLS_None);
			g_login_db.login_out((*it)->get_account_id(), (*it)->get_cur_ip());
			nlogin_player::free_login_player(*it);
			m_queuing_playeres.erase(it);
			return;
		}
	}

	// 找出在验证队列中指定玩家
	nlogin_player* nworld::get_proofing_player(uint64 client_id)
	{
		for( PlayerListIt it=m_proofing_playeres.begin(); m_proofing_playeres.end()!=it; ++it )
		{//玩家在验证
			if( (*it)->get_client_id() == client_id )
				return (*it);
		}

		return NULL;
	}

} // namespace nexus
