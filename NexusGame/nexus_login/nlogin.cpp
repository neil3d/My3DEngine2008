#include "nutil.h"
#include "nlogin.h"
#include "nlan_server.h"
#include "nmsg_dispatch.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nlogin_db.h"
#include "nworld_mgr.h"
#include "nplayer_account_mgr.h"
#include "ninner_cmd.h"


#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 100;

	volatile LONG nlogin::m_terminate = 0;

	nlogin::nlogin()
	{
	}

	nlogin::~nlogin()
	{
	}

	bool nlogin::init()
	{
		NLOG_STRING(_T("nexus nlogin startup!"));
		// log 初始化
		if (false == NLOG.init(_T("nlogin"), ELT_Debug, ELT_Debug))
		{
			NLOG_ERROR(_T("%s, init log error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin log init success!"));

		// config 初始化
		if (false == sncfg.init(_T("nlogin.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin config init success!"));

		// nutil的初始化
		s_util.init();
		NLOG_STRING(_T("nexus ndb nutil init success!"));

		// ninner_cmd 初始化
		g_inner_cmd.init();
		NLOG_STRING(_T("nexus ndb ninner_cmd init success!"));

		// 启动数据库
		if(false == g_login_db.init(sncfg.get_string(ECI_DatabaseHost), sncfg.get_string(ECI_DatabasePort), sncfg.get_string(ECI_DatabaseName)))
		{
			NLOG_ERROR(_T("%s, login_db init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin_db init success!"));

		// 启动nworld_mgr
		if(false == g_world_mgr.init())
		{
			NLOG_ERROR(_T("%s, nworld_mgr init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nworld_mgr init success!"));


		// 启动nplayre_mgr
		if(false == g_player_account_mgr.init())
		{
			NLOG_ERROR(_T("%s, nplayer_mgr init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nplayer_mgr init success!"));

		// 消息注册
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin message dispatch init success!"));

		uint32 value = sncfg.get_uint32(ECI_MaxLanSession);
		// nlan_server 初始化,注意先初始化再startup
		if (false == g_lan_server.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin LAN server init success!"));

		uint32 port = sncfg.get_uint32(ECI_LanPort);
		// LAN启动
		if (false == g_lan_server.startup(port, value))
		{
			NLOG_ERROR(_T("%s, nlan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nlogin LAN server startup success!"));

		// 初始化本类相关信息

		// 启动update线程
		m_terminate = 0;
		m_threads.create_thread( boost::bind(&nlogin::_update_thread, this) );

		NLOG_STRING(_T("nexus nlogin update thread init success!"));

		m_threads.create_thread( boost::bind(&nlogin::_cmd_line_thread, this) );
		NLOG_STRING(_T("nexus ninner_cmd cmd_line_thread success!"));

		return true;
	}

	void nlogin::destroy()
	{
		// update线程退出
		m_terminate = 1;
		m_threads.join_all();
		NLOG_STRING(_T("nexus nlogin update thread close!"));
		// WAN关闭
		g_lan_server.shutdown();
		NLOG_STRING(_T("nexus nlogin LAN server shutdown!"));
		// wan session 销毁
		g_lan_server.destroy();
		NLOG_STRING(_T("nexus nlogin LAN server destroy!"));
		// 消息派发
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus nlogin message dispatch destroy!"));
		// nplayer_mgr销毁
		g_player_account_mgr.destroy();
		NLOG_STRING(_T("nexus nplayer_mgr destroy!"));
		// nworld_mgr销毁
		g_world_mgr.destroy();
		NLOG_STRING(_T("nexus nworld_mgr destroy!"));
		// Database关闭
		g_login_db.destroy();
		NLOG_STRING(_T("nexus nlogin_db destroy!"));
		// ninner_cmd销毁注册的命令
		g_inner_cmd.destroy();
		NLOG_STRING(_T("nexus nlogin ninner_cmd destroy!"));
		// nutil的销毁
		s_util.destroy();
		NLOG_STRING(_T("nexus nlogin nutil destroy!"));
		// 配置文件退出
		sncfg.destroy();
		NLOG_STRING(_T("nexus nlogin config destroy!"));
		// log退出
		NLOG.destroy();
	}

	bool nlogin::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void nlogin::stop()
	{
		m_terminate = 1;
	}

	void nlogin::_update_thread()
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

			g_lan_server.update(elapse);
			g_world_mgr.update(elapse);
			g_inner_cmd.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void nlogin::_cmd_line_thread(void)
	{
		nchar command_buf[256];

		while (0 == m_terminate)
		{
			memset(command_buf, 0, sizeof(command_buf));
			std::tcout.flush();

			std::tcin.getline(command_buf, sizeof(command_buf)-1);

			if (command_buf[0] != _T('0'))
			{
				for(uint32 i = 0; command_buf[i]; ++i)
				{
					if (command_buf[i] == _T('\r') || command_buf[i] == _T('\n'))
					{
						command_buf[i] = 0;
						break;
					}
				}

				if (0 == (*command_buf))
				{
					std::tcout << _T("\nnexus>");
					continue;
				}

				tstring exit_flag(command_buf);
				if( exit_flag==tstring(_T(".exit")) )
				{
					stop();
					break;
				}

				m_cmd_queue.enqueue(command_buf);
			}
		}
	}

	void nlogin::_on_signal(int s)
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

	void nlogin::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void nlogin::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}

} // namespace nexus
