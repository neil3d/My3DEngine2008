#include "nworld.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nmsg_dispatch.h"
#include "nworld_session.h"
#include "nlan_client.h"
#include "nlan_server.h"
#include "ntimer.h"
#include "nutil.h"
#include "nmap_mgr.h"
#include "ngm_commands.h"
#include "ninfo_data.h"

#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 80;
	volatile LONG nworld::m_terminate = 0;

	nworld::nworld()
	{
	}

	nworld::~nworld()
	{
	}

	bool nworld::init()
	{
		NLOG_STRING(_T("nexus nworld startup!"));
		// log 初始化
		if (false == NLOG.init(_T("nworld"), ELT_Debug, ELT_Debug))
		{
			std::cout << "init log error!" <<std::endl;
			return false;
		}

		NLOG_STRING(_T("nexus nworld log init success!"));

		// config 初始化
		if (false == sncfg.init(_T("nworld.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld config init success!"));

		// 游戏逻辑初始化（策划数据，地图创建等等）
		if (false == _init_game())
		{
			NLOG_ERROR(_T("%s, _init game error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld init game success!"));

		// 消息派发 初始化
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld message dispatch init success!"));

		// nworld_session 初始化
		if (false == g_world_session.init())
		{
			NLOG_ERROR(_T("%s, nworld_session init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld world session init success!"));

		uint32 value = sncfg.get_uint32(ECI_MaxLanSessionClient);

		if (false == g_lan_client.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld LAN client init success!"));

		value = sncfg.get_uint32(ECI_MaxLanSessionServer);

		if (false == g_lan_server.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld LAN server init success!"));

		value = sncfg.get_uint32(ECI_MaxLanSessionClient);
		// client启动
		if (false == g_lan_client.startup(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld LAN client startup success!"));

		// server启动
		uint16 port = uint16(sncfg.get_uint32(ECI_WorldPort));
		value = sncfg.get_uint32(ECI_MaxLanSessionServer);

		if (false == g_lan_server.startup(port, value))
		{
			NLOG_ERROR(_T("%s, nlan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld LAN server startup success!"));

		// 初始化本类相关信息

		// 启动update线程
		m_terminate = 0;
		m_update_thread_ptr.reset(new boost::thread(boost::bind(&nworld::_update_thread, this)));

		NLOG_STRING(_T("nexus nworld update thread init success!"));

		if (false == g_lan_client.connect_to_nlogin())
		{
			NLOG_ERROR(_T("%s, connect nlogin request error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld connect to login ..."));

		if (false == g_lan_client.connect_to_nbg())
		{
			NLOG_ERROR(_T("%s, connect nbg request error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld connect to bg ..."));

		return true;
	}

	void nworld::destroy()
	{
		// update线程退出
		m_terminate = 1;
		
		if (m_update_thread_ptr)
		{
		m_update_thread_ptr->join();
		}

		NLOG_STRING(_T("nexus nworld update thread close!"));
		// server关闭
		g_lan_server.shutdown();
		NLOG_STRING(_T("nexus nworld LAN server shutdown!"));
		// client关闭
		g_lan_client.shutdown();
		NLOG_STRING(_T("nexus nworld LAN client shutdown!"));
		// server销毁
		g_lan_server.destroy();
		NLOG_STRING(_T("nexus nworld LAN server destroy!"));
		// client销毁
		g_lan_client.destroy();
		NLOG_STRING(_T("nexus nworld LAN client destroy!"));
		// world_session销毁
		g_world_session.destroy();
		NLOG_STRING(_T("nexus nworld world session destroy!"));
		// 消息派发
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus nworld message dispathc destroy!"));
		// 游戏逻辑资源释放
		_destroy_game();
		NLOG_STRING(_T("nexus nworld game destroy!"));
		// 配置文件退出
		sncfg.destroy();
		NLOG_STRING(_T("nexus nworld config destroy!"));
		// log退出
		NLOG.destroy();
	}

	bool nworld::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void nworld::stop()
	{
		m_terminate = 1;
	}

	bool nworld::_init_game()
	{
		if (false == g_game_timer.init())
		{
			NLOG_ERROR(_T("%s, ngame_timer init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld ngame_timer init success!"));

		if (false == s_util.init())
		{
			NLOG_ERROR(_T("%s, util init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld util init success!"));


		if (false == load_info_data())
		{
			NLOG_ERROR(_T("%s, load_info_data error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld load_info_data success!"));

		if (false == g_map_mgr.init())
		{
			NLOG_ERROR(_T("%s, nmap_mgr init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld nmap_mgr init success!"));

		if (false == s_gm_command.init())
		{
			NLOG_ERROR(_T("%s, ngm_command init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld ngm_command init success!"));
		return true;
	}

	void nworld::_destroy_game()
	{
		s_gm_command.destroy();
		g_map_mgr.destroy();
		s_util.destroy();
		g_game_timer.destroy();
	}

	void nworld::_update_thread()
	{
		::timeBeginPeriod(1);
		uint32 elapse = 0;
		uint32 cur_time = 0;
		uint32 pre_time = ::timeGetTime();

		while (0 == m_terminate)
		{
			cur_time = ::timeGetTime();
			elapse = cur_time - pre_time;
			pre_time = cur_time;

			// 注意线程安全, 注意顺序
			g_game_timer.update(elapse);
			g_world_session.update(elapse);
			g_map_mgr.update(elapse);
			g_world_session.update_world_msg(elapse);
			g_lan_server.update(elapse);
			g_lan_client.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void nworld::_on_signal(int s)
	{
		switch (s)
		{
		case SIGINT:
		case SIGTERM:
		case SIGBREAK:
			{
				stop();
			}
			break;
		}

		signal(s, _on_signal);
	}

	void nworld::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void nworld::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}


} // namespace nexus
