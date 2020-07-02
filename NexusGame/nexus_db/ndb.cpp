#include "nutil.h"
#include "ndb.h"
#include "nstore_center.h"
#include "ncache_center.h"
#include "nlan_client.h"
#include "nmsg_dispatch.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "ninner_cmd.h"

#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 100;

	volatile LONG ndb::m_terminate = 0;

	ndb::ndb()
	{
	}

	ndb::~ndb()
	{
	}

	bool ndb::init()
	{
		NLOG_STRING(_T("nexus ndb startup!"));
		// log 初始化
		if (false == NLOG.init(_T("ndb"), ELT_Debug, ELT_Debug))
		{
			NLOG_ERROR(_T("%s, init log error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb log init success!"));

		// config 初始化
		if (false == sncfg.init(_T("ndb.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb config init success!"));

		// nutil的初始化
		s_util.init();
		NLOG_STRING(_T("nexus ndb nutil init success!"));

		// ninner_cmd 初始化
		g_inner_cmd.init();
		NLOG_STRING(_T("nexus ndb ninner_cmd init success!"));

		// 启动数据库
		if(false == g_nexus_game_db.init(sncfg.get_string(ECI_DatabaseHost), sncfg.get_string(ECI_DatabasePort), sncfg.get_string(ECI_DatabaseName)))
		{
			NLOG_ERROR(_T("%s, nexus_game_db init error!"), __TFUNCTION__);
			return false;
		}

		//启动数据库缓存中心
		g_cache_center.start();

		// 消息注册
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb message dispatch init success!"));

		uint32 value = sncfg.get_uint32(ECI_MaxLanSession);
		// nlan_client 初始化,注意先初始化再startup
		if (false == g_lan_client.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb LAN client init success!"));

		// LAN启动
		if (false == g_lan_client.startup(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb LAN client startup success!"));

		// 初始化本类相关信息


		// 启动update线程
		m_terminate = 0;
		m_threads.create_thread( boost::bind(&ndb::_update_thread, this) );

		NLOG_STRING(_T("nexus ndb update thread init success!"));

		m_threads.create_thread( boost::bind(&ndb::_cmd_line_thread, this) );
		NLOG_STRING(_T("nexus ninner_cmd cmd_line_thread success!"));

		// 通过配置文件ndb只连接nworld
		if (false == g_lan_client.connect_to_nworld())
		{
			NLOG_ERROR(_T("%s, connect nworld request error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ndb connect to world ..."));

		return true;
	}

	void ndb::destroy()
	{
		// update线程退出
		m_terminate = 1;
		m_threads.join_all();
		NLOG_STRING(_T("nexus ndb update thread close!"));
		// WAN关闭
		g_lan_client.shutdown();
		NLOG_STRING(_T("nexus ndb LAN server shutdown!"));
		// wan session 销毁
		g_lan_client.destroy();
		NLOG_STRING(_T("nexus ndb LAN server destroy!"));
		// 消息派发
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus ndb message dispatch destroy!"));
		//关闭数据缓存中心
		g_cache_center.destroy();
		// Database关闭
		g_nexus_game_db.destroy();
		NLOG_STRING(_T("nexus game db destroy!"));
		// ninner_cmd销毁注册的命令
		g_inner_cmd.destroy();
		// nutil的销毁
		s_util.destroy();
		NLOG_STRING(_T("nexus nlogin nutil destroy!"));
		// 配置文件退出
		sncfg.destroy();
		NLOG_STRING(_T("nexus ndb config destroy!"));
		// log退出
		NLOG.destroy();
	}

	bool ndb::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void ndb::stop()
	{
		m_terminate = 1;
	}

	void ndb::_update_thread()
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

			g_lan_client.update(elapse);
			g_cache_center.update(elapse);
			g_inner_cmd.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void ndb::_cmd_line_thread(void)
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
				}

				m_cmd_queue.enqueue(command_buf);
			}
		}
	}

	void ndb::_on_signal(int s)
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

	void ndb::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void ndb::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}

} // namespace nexus
