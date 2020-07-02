#include "ngateway.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nmsg_dispatch.h"
#include "nlan_client.h"
#include "nwan_server.h"

#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 200;

	volatile LONG ngateway::m_terminate = 0;

	ngateway::ngateway()
	{
	}

	ngateway::~ngateway()
	{
	}

	bool ngateway::init()
	{
		NLOG_STRING(_T("nexus ngateway startup!"));
		// log 初始化
		if (false == NLOG.init(_T("ngateway"), ELT_Debug, ELT_Debug))
		{
			std::cout << "init log error!" <<std::endl;
			return false;
		}

		NLOG_STRING(_T("nexus ngateway log init success!"));

		// config 初始化
		if (false == sncfg.init(_T("ngateway.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway config init success!"));

		// 消息注册
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway message dispatch init success!"));

		uint32 value = sncfg.get_uint32(ECI_MaxLanSession);
		// nlan_client 初始化,注意先初始化再startup
		if (false == g_lan_client.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway LAN client init success!"));

		value = sncfg.get_uint32(ECI_MaxSession);
		// nwan_server 初始化,注意先初始化再startup
		if (false == g_wan_server.init(value))
		{
			NLOG_ERROR(_T("%s, nwan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway WAN server init success!"));

		value = sncfg.get_uint32(ECI_MaxLanSession);
		// LAN启动
		if (false == g_lan_client.startup(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway LAN client startup success!"));

		// WAN启动
		ntcp_server_config param;
		param.port = uint16(sncfg.get_uint32(ECI_WanPort));
		param.max_session = sncfg.get_uint32(ECI_MaxSession);
		param.minilzo = (sncfg.get_int32(ECI_Minilzo) == 1) ? true : false;

		if (false == g_wan_server.startup(param))
		{
			NLOG_ERROR(_T("%s, nwan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway WAN server startup success!"));

		// 初始化本类相关信息

		// 启动update线程
		m_terminate = 0;
		m_update_thread_ptr.reset(new boost::thread(boost::bind(&ngateway::_update_thread, this)));

		NLOG_STRING(_T("nexus ngateway update thread init success!"));

		// 通过配置文件ngateway只连接nworld, nlogin和nbg是由nworld返回相应ip和port后在连接
		if (false == g_lan_client.connect_to_nworld())
		{
			NLOG_ERROR(_T("%s, connect nworld request error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway connect to world ..."));
		return true;
	}

	void ngateway::destroy()
	{
		// update线程退出
		m_terminate = 1;
		m_update_thread_ptr->join();
		NLOG_STRING(_T("nexus ngateway update thread close!"));
		// WAN关闭
		g_wan_server.shutdown();
		NLOG_STRING(_T("nexus ngateway WAN server shutdown!"));
		// LAN关闭
		g_lan_client.shutdown();
		NLOG_STRING(_T("nexus ngateway LAN client shutdown!"));
		// wan session 销毁
		g_wan_server.destroy();
		NLOG_STRING(_T("nexus ngateway WAN server destroy!"));
		// lan session 销毁
		g_lan_client.destroy();
		NLOG_STRING(_T("nexus ngateway LAN client destroy!"));
		// 消息派发
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus ngateway message dispatch destroy!"));
		// 配置文件退出
		sncfg.destroy();
		NLOG_STRING(_T("nexus ngateway config destroy!"));
		// log退出
		NLOG.destroy();
	}

	bool ngateway::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void ngateway::stop()
	{
		m_terminate = 1;
	}

	void ngateway::_update_thread()
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

			g_wan_server.update(elapse);
			g_lan_client.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void ngateway::_on_signal(int s)
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

	void ngateway::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void ngateway::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}

} // namespace nexus
