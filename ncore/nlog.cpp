#include "stdafx.h"
#include <stdarg.h>
#include <fstream>
using namespace std;

#include "nlog.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

namespace nexus
{
	struct nlog::impl
	{
		wofstream	out_file;
	};

	nlog::nlog(void):m_impl(nNew impl)
	{
	}

	nlog::~nlog(void)
	{
		delete m_impl;
	}

	void nlog::init(const nstring& file_name)
	{
		m_impl->out_file.open(file_name.c_str(), ios_base::out);
	}

	static TCHAR _static_format_buf[4096] = {0};
	
	nstring	format_string(const TCHAR* fmt, ...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		return nstring(_static_format_buf);
	}

	void nlog::write(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		(m_impl->out_file) << _static_format_buf;
		m_impl->out_file.flush();
	}

	void nlog::write_info(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		(m_impl->out_file) << _T("Info : ") << _static_format_buf;
		m_impl->out_file.flush();
	}

	void nlog::write_warning(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		(m_impl->out_file) << _T("Warning : ") << _static_format_buf;
		m_impl->out_file.flush();
	}

	void nlog::write_error(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		(m_impl->out_file) << _T("Error : ") << _static_format_buf;
		m_impl->out_file.flush();
	}

	void nlog::write_exception(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		(m_impl->out_file) << endl <<_T("Exception : ") << _static_format_buf << endl;
		m_impl->out_file.flush();
	}

	void nlog::write_exception(const char* std_exp)
	{
		(m_impl->out_file) << endl <<_T("Exception : ") << std_exp << endl;
		m_impl->out_file.flush();
	}

	void nlog::trace(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		::OutputDebugString(_static_format_buf);
	}
}//namespace nexus