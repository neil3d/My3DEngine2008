#include "nc_client.h"
#include "nc_session.h"
#include "nwinsock_init.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	nc_client::nc_client() : m_completion_port(0), m_status(TSVRS_NONE), m_sessions_ptr(NULL), m_thread_num(0), m_max_session(0), 
		m_nagle(false), m_pools_ptr(NULL)
	{
	}

	nc_client::~nc_client()
	{
		_free_memory();
	}

	bool nc_client::startup(uint32 max_session, uint32 thread_num, uint32 max_buffer_size, bool nagle)
	{
		if (0 == max_session || 0 == thread_num || 0 == max_buffer_size)
		{
			NLOG_NERROR(_T("startup param error!, max_session = %u, thread_num = %u, max_buffer_size = %u"), max_session, thread_num, max_buffer_size);
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

		m_thread_num		= thread_num;
		m_max_session		= max_session;
		m_nagle				= nagle;

		m_sessions_ptr = new nc_session[m_max_session];

		if (!m_sessions_ptr)
		{
			NLOG_NERROR(_T("alloc nc_session memory error!"));
			return false;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_sessions_ptr[i]._init(uint16(i), NULL, this, max_buffer_size, m_nagle))
			{
				_return_session(&(m_sessions_ptr[i]));
			}
		}

		m_pools_ptr = new nsafe_mem_pool[m_thread_num];

		if (!m_pools_ptr)
		{
			NLOG_NERROR(_T("alloc nsafe_mem_pool memory error!"));
			_free_memory();
			return false;
		}

		// 创建完成端口
		m_completion_port = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);

		// 创建完成包处理线程
		for (uint32 i = 0; i < m_thread_num; ++i)
		{
			m_threads.create_thread(boost::bind(&nc_client::_worker_thread, this, i));
		}

		// open状态
		m_status = TSVRS_OPEN;

		return true;
	}

	void nc_client::shutdown()
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
			m_sessions_ptr[i]._close();
		}

		bool exit = true;
		TCPSessionStatus status = TSS_NONE;

		while (true)
		{
			exit = true;

			for (uint32 i = 0; i < m_max_session; ++i)
			{
				status = m_sessions_ptr[i]._get_status();

				// TSS_ACCEPTEX等待变为TSS_CLOSE或TSS_OPEN，TSS_OPEN重新投递close请求，TSS_SHUT等待变为TSS_CLOSE
				if (TSS_ACCEPTEX == status || TSS_OPEN == status || TSS_SHUT == status)
				{
					exit = false;

					if (TSS_OPEN == status)
					{
						m_sessions_ptr[i]._close();			// 第一次投递的close请求失败，重新投递
					}
				}
			}

			::Sleep(100);

			if (exit)
			{
				break;
			}
		}
		// 关闭工作线程
		::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0XFFFFFFFF, 0);

		// 等待工作线程的退出
		m_threads.join_all();

		if (m_completion_port)
		{ 
			::CloseHandle(m_completion_port);
			m_completion_port = 0;
		}

		m_status = TSVRS_CLOSE;
		_free_memory();
	}

	uint32 nc_client::connect_to(const std::string& ip_addresss, uint16 port)
	{
		if (TSVRS_OPEN != m_status)
		{
			return 0XFFFFFFFF;
		}

		nc_session* session_ptr = _get_available_session();

		if (!session_ptr)
		{
			return 0XFFFFFFFF;
		}

		return session_ptr->connect_to(ip_addresss, port);
	}

	void nc_client::_free_memory()
	{
		// 注意内存的释放顺序, session里用到的内存池数据在归还池时释放
		if (m_sessions_ptr)
		{
			delete []m_sessions_ptr;
			m_sessions_ptr = NULL;
		}

		if (m_pools_ptr)
		{
			delete []m_pools_ptr;
			m_pools_ptr = NULL;
		}
	}

	void nc_client::_worker_thread(int32 thread_index)
	{
		BOOL			ok = 0;
		DWORD			bytes_transferred = 0;
		DWORD			last_error = 0;
		LPOVERLAPPED	overlapped_ptr = NULL;
		noverlappedex*	ov_ptr = NULL;
		nsafe_mem_pool*	pool_ptr = &(m_pools_ptr[thread_index]);

#if defined(WINVER) && (WINVER < 0x0500)
		DWORD			completion_key = 0;
#else
		DWORD_PTR		completion_key = 0;
#endif

		for (;;)
		{
			::WSASetLastError(0);
			ok = ::GetQueuedCompletionStatus(m_completion_port, &bytes_transferred, &completion_key, &overlapped_ptr, 500);
			last_error = ::WSAGetLastError();

			if (!ok && NULL == overlapped_ptr)
			{
				if (WAIT_TIMEOUT == last_error)
				{
					continue;
				}
				else
				{
					break;
				}
			}
			else if (NULL != overlapped_ptr)
			{
				ov_ptr = CONTAINING_RECORD(overlapped_ptr, noverlappedex, ov);

				switch (ov_ptr->status)
				{
				case IOCS_ACCEPT:				// CONNECTEX
					{
						reinterpret_cast<nc_session*>(completion_key)->_handle_connect(last_error);
					}
					break;
				case IOCS_READ:
					{
						reinterpret_cast<nc_session*>(completion_key)->_handle_read(last_error, bytes_transferred, pool_ptr);
					}
					break;
				case IOCS_WRITE:
					{
						reinterpret_cast<nc_session*>(completion_key)->_handle_write(last_error, bytes_transferred);
					}
					break;
				default:
					{
					}
					break;
				}
			}
			else if (0XFFFFFFFF == completion_key)
			{
				// Wake up next thread that is blocked on GetQueuedCompletionStatus.
				::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0XFFFFFFFF, 0);
				break;
			}
			else
			{
				// close的回调函数,保证所有的回调函数的所属线程都为iocp的工作线程，这样回调函数没有递归调用
				_handle_close(uint32(completion_key));
			}
		}
	}

} // namespace nexus
