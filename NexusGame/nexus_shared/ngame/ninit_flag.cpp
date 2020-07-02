#include "ninit_flag.h"
#include "nlan_server.h"
#include "ncfg.h"

#include "nmsg_world_gateway.h"

namespace nexus {

	ninit_flag::ninit_flag()
	{
		m_flag = EIF_Loin | EIF_Bg | EIF_Db;
	}

	ninit_flag::~ninit_flag()
	{
	}

	void ninit_flag::set_init_flag(uint32 flag)
	{
		m_flag &= ~flag;

		if (true == is_init_ok())
		{
			_send_gateway_login_info();
		}
	}

	void ninit_flag::gateway_login(uint32 session_id)
	{
		m_session_ids.push_back(session_id);

		if (true == is_init_ok())
		{
			_send_gateway_login_info();
		}
	}

	void ninit_flag::_send_gateway_login_info()
	{
		std::string login_ip = sncfg.get_string(ECI_NLoginIp);
		std::string bg_ip = sncfg.get_string(ECI_NBgIp);
		// 发送完后id清空
		tagMsg_G2W_Server_Info reply;

		reply.login_ip		= inet_addr(login_ip.c_str());
		reply.login_port	= uint16(sncfg.get_uint32(ECI_NLoginPort));
		reply.bg_ip			= inet_addr(bg_ip.c_str());
		reply.bg_port		= uint16(sncfg.get_uint32(ECI_NBgPort));

		for (ID_VECTOR::iterator iter = m_session_ids.begin(); iter != m_session_ids.end(); ++iter)
		{
			reply.server_id = sncfg.get_uint32(ECI_ServerId) | (((*iter + 1) & 0X000000FF) << 24);
			g_lan_server.send_msg(*iter, &reply, sizeof(reply));
		}

		m_session_ids.clear();
	}

	ninit_flag g_init_flag;

} // namespace nexus
