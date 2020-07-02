#include "nscheduler.h"

namespace nexus {

	nscheduler::nscheduler() : m_completion_port(NULL), m_event(NULL), m_nocompleted_opt(0)
	{
	}

	nscheduler::~nscheduler()
	{
	}

	bool nscheduler::startup()
	{
		// 自动激活
		m_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);

		m_completion_port = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);

		if (NULL == m_completion_port)
		{
			return false;
		}

		SYSTEM_INFO sys_info;
		::GetSystemInfo(&sys_info);
		uint32 thread_num = sys_info.dwNumberOfProcessors * 2 + 2;

		for (uint32 i = 0; i < thread_num; ++i)
		{
			m_threads.create_thread(boost::bind(&nscheduler::_worker_thread, this));
		}

		return true;
	}

	void nscheduler::shutdown()
	{
		wait();

		// 关闭工作线程
		::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0, 0);

		// 等待工作线程的退出
		m_threads.join_all();

		if (m_completion_port)
		{ 
			::CloseHandle(m_completion_port);
			m_completion_port = 0;
		}

		if (m_event)
		{
			::CloseHandle(m_event);
			m_event = NULL;
		}
	}

	void nscheduler::_worker_thread()
	{
		BOOL			ok = 0;
		DWORD			bytes_transferred = 0;
		DWORD			last_error = 0;
		LPOVERLAPPED	overlapped_ptr = NULL;

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
				noperation* op_ptr = static_cast<noperation*>(overlapped_ptr);
				op_ptr->do_completion();
				_distory_operation(op_ptr);

				// 修改未完成的操作计数
				if (0 == ::InterlockedDecrement((LPLONG)&m_nocompleted_opt))
				{
					::SetEvent(m_event);
				}
			}
			else if (0 == completion_key)
			{
				// Wake up next thread that is blocked on GetQueuedCompletionStatus.
				::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0, 0);
				break;
			}
		}
	}

} // namespace nexus
