#include "ncluster_client.h"
#include "nguard.h"
#include "nwinsock_init.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

/**
	in http://tangentsoft.net/wskfaq/newbie.html#howclose 

	The proper sequence for closing a TCP connection is:
	1. Finish sending data. 
	2. Call shutdown() with the how parameter set to 1. 
	3. Loop on recv() until it returns 0. 
	4. Call closesocket(). 

	in MSDN:
	To assure that all data is sent and received on a connected socket 
	before it is closed, an application should use shutdown to close connection 
	before calling closesocket. For example, to initiate a graceful disconnect:
	1. Call WSAAsyncSelect to register for FD_CLOSE notification. 
	2. Call shutdown with how=SD_SEND. 
	3. When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
	4. Call closesocket. 
*/

namespace nexus {

	ncluster_client::ncluster_client() : m_status(TSVRS_NONE), m_sessions_ptr(NULL), m_terminate_connect(0), m_terminate_close(0), 
		m_max_session(0), m_nagle(false)
	{
	}

	ncluster_client::~ncluster_client()
	{
		_free_memory();
	}

	bool ncluster_client::startup(uint32 max_session, uint32 max_send_buffer, uint32 time_out_sec, bool nagle)
	{
		if (0 == max_session || 0 == max_send_buffer || 0 == time_out_sec)
		{
			NLOG_NERROR(_T("startup param error!, max_session = %u, max_send_buffer = %u, time_out_sec = %u, nagle = %u"), 
				max_session, max_send_buffer, time_out_sec, nagle);
			return false;
		}

		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_NONE != m_status)
			{
				return false;
			}

			m_status = TSVRS_INIT;
		}

		m_max_session	= max_session;				// 最大连接数
		m_time_out_sec	= time_out_sec;				// 超时设置单位s
		m_nagle			= nagle;					// 是否开启nagle算法

		m_sessions_ptr = new ncluster_session[m_max_session];

		if (!m_sessions_ptr)
		{
			NLOG_NERROR(_T("alloc ncluster_session memory error!"));
			return false;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_sessions_ptr[i]._init(uint16(i), NULL, this, max_send_buffer))
			{
				_return_session(&(m_sessions_ptr[i]));
			}
		}

		// 创建connect线程
		m_threads.create_thread(boost::bind(&ncluster_client::_thread_connect, this));

		// 创建close线程
		m_threads.create_thread(boost::bind(&ncluster_client::_thread_close, this));

		m_status = TSVRS_OPEN;

		return true;
	}

	void ncluster_client::shutdown()
	{
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status)
			{
				return;
			}

			// 关闭请求
			m_status = TSVRS_SHUT;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			m_sessions_ptr[i]._closing();
		}

		bool exit = true;
		TCPSessionStatus status = TSS_NONE;

		while (true)
		{
			exit = true;

			for (uint32 i = 0; i < m_max_session; ++i)
			{
				status = m_sessions_ptr[i]._get_status();

				// TSS_ACCEPTEX(请求connect状态)等待变为TSS_CLOSE或TSS_OPEN，TSS_OPEN重新投递close请求，TSS_SHUT等待变为TSS_CLOSE
				if (TSS_ACCEPTEX == status || TSS_OPEN == status || TSS_SHUT == status)
				{
					exit = false;

					if (TSS_OPEN == status)
					{
						m_sessions_ptr[i]._closing();			// 第一次投递的close请求失败，重新投递
					}
				}
			}

			::Sleep(100);

			if (exit)
			{
				break;
			}
		}
		// 关闭线程
		::InterlockedExchange((LONG*)&m_terminate_connect, 1);
		::InterlockedExchange((LONG*)&m_terminate_close, 1);

		// 等待工作线程的退出
		m_threads.join_all();

		m_status = TSVRS_CLOSE;
		_free_memory();
	}

	uint32 ncluster_client::connect_to(const std::string& ip_addresss, uint16 port)
	{
		if (TSVRS_OPEN != m_status || 1 == m_terminate_connect)
		{
			return 0XFFFFFFFF;
		}

		ncluster_session* session_ptr = _get_available_session();

		if (!session_ptr)
		{
			return 0XFFFFFFFF;
		}

		// 初始化connect的地址
		ZeroMemory(&session_ptr->m_address, sizeof(session_ptr->m_address));
		session_ptr->m_address.sin_addr.s_addr	= inet_addr(ip_addresss.c_str());
		session_ptr->m_address.sin_family		= AF_INET;
		session_ptr->m_address.sin_port			= htons(port);

		// 检查IP是否合法
		if (INADDR_NONE == session_ptr->m_address.sin_addr.s_addr)
		{
			_return_session(session_ptr);
			return 0XFFFFFFFF;
		}

		// 加入到connect队列
		_connecting_session(session_ptr);

		return session_ptr->_get_client_id();
	}
	void ncluster_client::_free_memory()
	{
		if (m_sessions_ptr)
		{
			delete []m_sessions_ptr;
			m_sessions_ptr = NULL;
		}
	}

	void ncluster_client::_thread_connect()
	{
		fd_set fdwrite;
		timeval block_time;						// 设置阻塞时间
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;		// 200 ms

		DWORD time_now		= 0;
		DWORD time_expired	= 0;

		int	result = 0;
		uint32 error_code = ECC_Max;
		ncluster_session* session_ptr = NULL;

		while (0 == m_terminate_connect || !m_connecting_sessions.empty())
		{
			if (0 == m_connecting_sessions.size())
			{
				::Sleep(200);
				continue;
			}

			session_ptr = _get_connnecting_session();

			if (!session_ptr)
			{
				::Sleep(200);
				continue;
			}

			time_now = ::timeGetTime();

			// 创建socket
			session_ptr->m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

			if (INVALID_SOCKET == session_ptr->m_socket)
			{
				_handle_connect(ECC_Error, session_ptr);
				continue;
			}

			// 使用非阻塞方式, linger行为
			DWORD arg = 1;
			result = ::ioctlsocket(session_ptr->m_socket, FIONBIO, &arg);

			if (SOCKET_ERROR == result)
			{
				_handle_connect(ECC_Error, session_ptr);
				continue;
			}

			// disables the Nagle algorithm for send coalescing
			if (!m_nagle)
			{
				BOOL nodelay = TRUE;
				::setsockopt(session_ptr->m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
			}

			// hard shut down
			linger lin;
			lin.l_linger	= 0;
			lin.l_onoff		= 1;
			::setsockopt(session_ptr->m_socket, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));

			// 连接到服务器
			// For connection-oriented, nonblocking sockets, it is often not possible 
			// to complete the connection immediately. In such a case, "connect"
			// returns the error WSAEWOULDBLOCK. However, the operation proceeds.
			result = ::connect(session_ptr->m_socket, (sockaddr*)&(session_ptr->m_address), sizeof(session_ptr->m_address));

			if (SOCKET_ERROR == result)
			{	
				if (WSAEWOULDBLOCK == ::WSAGetLastError())
				{
					// 不停重试, 直到超时为止
					while (true)
					{
						FD_ZERO(&fdwrite);
						FD_SET(session_ptr->m_socket, &fdwrite);

						if (1 == select(0, NULL, &fdwrite, NULL, &block_time))
						{
							// 连接成功
							error_code = ECC_Connected;
							break;
						}

						if (::timeGetTime() - time_now >= m_time_out_sec * 1000)
						{
							error_code = ECC_ConnectTimeout;
							break;
						}

						if (m_terminate_connect)
						{
							error_code = ECC_ConnectAborted;
							break;
						}
					}
				}
				else
				{
					error_code = ECC_Error;
				}
			}

			_handle_connect(error_code, session_ptr);
				}
				}

	void ncluster_client::_thread_close()
	{
		ncluster_session* session_ptr = NULL;

		while (0 == m_terminate_close || !m_closing_sessions.empty())
		{
			if (m_closing_sessions.empty())
			{
				::Sleep(200);
				continue;
			}

			session_ptr = _get_closing_session();

			if (!session_ptr)
			{
				::Sleep(200);
				continue;
			}

			session_ptr->_close();
		}
	}

	void ncluster_client::_handle_connect(uint32 error_code, ncluster_session* session_ptr)
	{
		switch (error_code)
		{
		case ECC_Connected:
			{
				session_ptr->_open();
			}
			break;
		default:
			{
				session_ptr->_set_status(TSS_CLOSE);
				on_connect(session_ptr->_get_client_id(), error_code, 0);

				if (0 != session_ptr->m_socket)
				{
					::closesocket(session_ptr->m_socket);
					session_ptr->m_socket = 0;
				}

				_return_session(session_ptr);
			}
			break;
		}
	}

} // namespace nexus
