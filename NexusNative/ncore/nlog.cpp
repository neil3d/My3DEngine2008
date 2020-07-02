#include "stdafx.h"
#include "util/string_util.h"
#include <stdarg.h>
#include <fstream>
using namespace std;

#include "nlog.h"
#include "object/nclass.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

namespace nexus
{
	//-- class nlog_file ----------------------------------------------------------------
	nDEFINE_VIRTUAL_CLASS(nlog_output, nobject);
	class nlog_file : public nlog_output
	{
		std::wofstream m_file;
	public:
		nlog_file(void)	{}
		virtual ~nlog_file(void)	{}
		
		virtual void open(const nstring& param)
		{
			m_file.open(param.c_str(), ios_base::out);
		}

		virtual void write_string(const TCHAR* txt)
		{
			m_file << txt;
		}

		virtual void write_string(enum ELogType t, const TCHAR* txt)
		{
			switch(t)
			{
			case ELog_Info:
				m_file << _T("Info : ") << txt;
				break;
			case ELog_Warning:
				m_file << _T("Warning : ") << txt;
				break;
			case ELog_Error:
				m_file << _T("Error : ") << txt;
				break;
			case ELog_Exception:
				m_file << std::endl << _T("Exception : ") << txt << std::endl;
				break;
			default:
				m_file << t << txt;
				break;
			}
			m_file.flush();
		}

		virtual void close()
		{
			m_file.close();
		}

		nDECLARE_CLASS(nlog_file)
	};	
	nDEFINE_CLASS(nlog_file, nlog_output)

	//-- class nlog ----------------------------------------------------------------
	nlog::nlog(void)
	{
	}

	nlog::~nlog(void)
	{		
	}

	void nlog::init(const nstring& out_class_name, const nstring& param)
	{
		boost::shared_ptr<nlog_output> out_ptr
			( nconstruct<nlog_output>(out_class_name) );
		out_ptr->open( param);

		m_out_device = out_ptr;
		m_default_out = out_ptr;
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

		m_out_device->write_string(_static_format_buf);		
	}

	void nlog::write_info(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		m_out_device->write_string(ELog_Info, _static_format_buf);		
	}

	void nlog::write_warning(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		m_out_device->write_string(ELog_Warning, _static_format_buf);		
	}

	void nlog::write_error(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		m_out_device->write_string(ELog_Error, _static_format_buf);		
	}

	void nlog::write_exception(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		m_last_exception = nstring(_static_format_buf);
		m_out_device->write_string(ELog_Exception, _static_format_buf);		
	}

	void nlog::write_exception(const char* std_exp)
	{		
		m_last_exception = conv_string(std::string(std_exp));
		m_out_device->write_string(ELog_Exception, m_last_exception.c_str());		
	}

	void nlog::trace(const TCHAR* fmt,...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		::OutputDebugString(_static_format_buf);
	}

	nstring nlog::get_last_exception()
	{
		return m_last_exception;
	}

	void nlog::redirect(boost::shared_ptr<nlog_output> new_output)
	{
		if(new_output)
			m_out_device = new_output;
		else
		{
			m_out_device = m_default_out;
		}
	}
}//namespace nexus