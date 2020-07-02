#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <time.h>
#include <iostream>

namespace nexus {

	const uint32 MAX_MSG_LENGTH = 500;

	nlog_mt::nlog_mt() : m_terminate(1), m_log_msg(0), m_head_ptr(NULL), m_tail_ptr(NULL), m_screen_log_level(0), m_file_log_level(0), 
		m_colored(false), m_include_time(false), m_stdout_handle(NULL)
	{
		ZeroMemory(m_color, sizeof(m_color));
		ZeroMemory(m_win_color_fg, sizeof(m_win_color_fg));
	}

	nlog_mt::~nlog_mt()
	{
	}

	bool nlog_mt::init(const tstring& log_name, uint32 screen_log_level, uint32 file_log_level, bool color, bool time)
	{
		m_log_name			= log_name;
		m_screen_log_level	= screen_log_level;
		m_file_log_level	= file_log_level;
		m_colored			= color;
		m_include_time		= time;

		m_win_color_fg[ECT_White]	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		m_win_color_fg[ECT_Red]		= FOREGROUND_RED | FOREGROUND_INTENSITY;
		m_win_color_fg[ECT_Pink]	= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		m_win_color_fg[ECT_Green]	= FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		m_win_color_fg[ECT_Blue]	= FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		m_win_color_fg[ECT_Yellow]	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

		m_stdout_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

		m_node_str[ELT_Normal]	= _T(", ");
		m_node_str[ELT_Fatal]	= _T("_fatal, ");
		m_node_str[ELT_NError]	= _T("_net_error, ");
		m_node_str[ELT_Error]	= _T("_error, ");
		m_node_str[ELT_Warning]	= _T("_warning, ");
		m_node_str[ELT_Info]	= _T("_info, ");
		m_node_str[ELT_Debug]	= _T("_debug, ");

		m_color[ELT_Normal]		= ECT_White;
		m_color[ELT_Fatal]		= ECT_Red;
		m_color[ELT_NError]		= ECT_Pink;
		m_color[ELT_Error]		= ECT_Yellow;
		m_color[ELT_Warning]	= ECT_Blue;
		m_color[ELT_Info]		= ECT_White;
		m_color[ELT_Debug]		= ECT_Green;

		// 打开文件
		nchar cur_path[MAX_PATH] = {0};
		DWORD result = GetCurrentDirectory(MAX_PATH, cur_path);

		tstring path(cur_path);
		path += _T("/log");
		CreateDirectory(path.c_str(), NULL);

		tstring file_name = path + _T("/") + log_name + _T("_") + _get_timestamp() + _T(".log");
		m_app_file.open(file_name.c_str(), std::ios_base::out);
		file_name = path + _T("/") + log_name + _T("_net_") + _get_timestamp() + _T(".log");
		m_net_file.open(file_name.c_str(), std::ios_base::out);

		m_terminate = 0;
		// 创建工作线程
		m_worker_thread_ptr.reset(new boost::thread(boost::bind(&nlog_mt::_worker_thread, this)));

		return true;
	}

	void nlog_mt::destroy()
	{
		// 退出工作线程
		m_terminate = 1;
		m_worker_thread_ptr->join();

		// 关闭文件
		if (m_app_file.is_open())
		{
			m_app_file.flush();
			m_app_file.close();
		}

		if (m_net_file.is_open())
		{
			m_net_file.flush();
			m_net_file.close();
		}
	}

	void nlog_mt::out_string(const nchar* str, ...)
	{
		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Normal;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_fatal(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_Fatal && m_file_log_level < ELT_Fatal)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Fatal;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_net_error(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_NError && m_file_log_level < ELT_NError)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_NError;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_error(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_Error && m_file_log_level < ELT_Error)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Error;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_warning(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_Warning && m_file_log_level < ELT_Warning)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Warning;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_info(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_Info && m_file_log_level < ELT_Info)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Info;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::out_debug(const nchar* str, ...)
	{
		if (m_screen_log_level < ELT_Debug && m_file_log_level < ELT_Debug)
		{
			return;
		}

		if (5000 < m_log_msg || 0 != m_terminate || !str)
		{
			return;
		}

		nlog_msg* log_msg_ptr = reinterpret_cast<nlog_msg*>(m_pool.alloc(MAX_MSG_LENGTH * sizeof(nchar) + 1));

		if (!log_msg_ptr)
		{
			return;
		}

		log_msg_ptr->next_ptr = NULL;
		log_msg_ptr->log_type = ELT_Debug;

		nchar* buf_ptr = log_msg_ptr->buffer;

		va_list ap;
		va_start(ap, str);
		int len = _vsctprintf(str, ap) + 1;

		if (len > MAX_MSG_LENGTH * sizeof(nchar) + 1)
		{
			va_end(ap);
			return;
		}

		_vstprintf_s(buf_ptr, len, str, ap);
		va_end(ap);

		_push_log_msg(log_msg_ptr);
	}

	void nlog_mt::_set_color(uint32 color)
	{
		::SetConsoleTextAttribute(m_stdout_handle, m_win_color_fg[color]);
	}

	void nlog_mt::_reset_color()
	{
		::SetConsoleTextAttribute(m_stdout_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	std::wstring nlog_mt::_get_timestamp()
	{
		time_t t = time(NULL);
		tm aTm;
		localtime_s(&aTm, &t);
		//	YYYY	year
		//	MM  	month	(2 digits 01-12)
		//	DD  	day		(2 digits 01-31)
		//	HH  	hour	(2 digits 00-23)
		//	MM  	minutes	(2 digits 00-59)
		//	SS  	seconds	(2 digits 00-59)
		nchar buf[20];
		_sntprintf_s(buf, 20, _T("%04d-%02d-%02d_%02d-%02d-%02d"), aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_min, aTm.tm_sec);
		return tstring(buf);
	}

	void nlog_mt::_worker_thread()
	{
		nlog_msg* cur_log_ptr = NULL;
		nlog_msg* pre_log_ptr = NULL;
		uint32 i = 0;

		while (0 == m_terminate || cur_log_ptr || m_head_ptr)
		{
			// 取出投递的请求log msg
			if (!cur_log_ptr)
			{
				if (!m_head_ptr)
				{
					::Sleep(100);
					continue;
				}

				{
					nguard<nfast_mutex> guard(m_mutex);
					cur_log_ptr = m_head_ptr;
					m_head_ptr = NULL;
					m_tail_ptr = NULL;
				}
			}

			i = 0;

			while (cur_log_ptr)
			{
				if (100 == ++i)
				{
					break;
				}

				_handle_log_msg(cur_log_ptr);

				pre_log_ptr = cur_log_ptr;
				cur_log_ptr = cur_log_ptr->next_ptr;
				// free to pool
				m_pool.free(pre_log_ptr);
			}

			::Sleep(100);
		}
	}

	void nlog_mt::_push_log_msg(nlog_msg* log_msg_ptr)
	{
		nguard<nfast_mutex> guard(m_mutex);

		if (!m_tail_ptr)
		{
			m_head_ptr = log_msg_ptr;
		}
		else
		{
			m_tail_ptr->next_ptr = log_msg_ptr;
		}

		m_tail_ptr = log_msg_ptr;
		::InterlockedIncrement((LPLONG)&m_log_msg);
	}

	void nlog_mt::_handle_log_msg(nlog_msg* log_msg_ptr)
	{
		if (!log_msg_ptr)
		{
			return;
		}

		// log screen
		if (m_colored)
		{
			_set_color(m_color[log_msg_ptr->log_type]);
		}

		tstring prefix = _T("");

		if (m_include_time)
		{
			prefix += _get_timestamp() + _T("_");
		}

		prefix += m_log_name + m_node_str[log_msg_ptr->log_type];

		if (m_screen_log_level >= log_msg_ptr->log_type)
		{
#ifdef _UNICODE
			std::wcout << prefix << log_msg_ptr->buffer << std::endl;
			std::wcout.flush();
#else
			std::cout << prefix << log_msg_ptr->buffer << std::endl;
			std::cout.flush();
#endif
		}

		if (m_colored)
		{
			_reset_color();
		}

		// log file
		if (m_file_log_level >= log_msg_ptr->log_type)
		{
			if (ELT_NError == log_msg_ptr->log_type)
			{
				m_net_file << prefix << log_msg_ptr->buffer << std::endl;
				m_net_file.flush();
			}
			else
			{
				m_app_file << prefix << log_msg_ptr->buffer << std::endl;
				m_app_file.flush();
			}
		}

		::InterlockedDecrement((LPLONG)&m_log_msg);
	}

} // namespace nexus
