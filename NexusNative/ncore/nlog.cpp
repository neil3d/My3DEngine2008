#include "stdafx.h"
#include "util/string_util.h"
#include <stdarg.h>
#include <iostream>
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

	nCoreAPI void format_log_message( std::wostream& out, enum ELogType type, const TCHAR* msg )
	{
		out << "[";
		if (type >= ELog_MAX)
		{
			out << "(";
			out << boost::lexical_cast<std::wstring>(type);
			out << ")";
		}
		else
		{
			static TCHAR* severity_names[ELog_MAX] =
			{
				_T("Info"),
				_T("Warning"),
				_T("Error"),
				_T("Exception"),
				_T("Debug"),
			};

			out << severity_names[type];
		}
		out << "]";

		out << msg << std::endl;
	}

	nlog_output::~nlog_output( void )
	{
		//nlog::instance()->remove_log_output(this);
	}

	//-- class nlog ----------------------------------------------------------------
	nlog::~nlog(void)
	{		
	}

	void nlog::add_log_output( log_output_ptr new_output )
	{
		if(!new_output.get())
		{
			return;
		}

		m_target_outputs.push_back(new_output);
	}

	void nlog::remove_log_output( log_output_ptr output )
	{
		if(!output.get())
			return;

		if(m_target_outputs.empty())
			return;
		m_target_outputs.erase(std::find(m_target_outputs.begin(),m_target_outputs.end(),output));
	}


	void nlog::clear_log_output()
	{
		m_target_outputs.clear();
	}

	void nlog::enable_log_type( const ELogType t, const bool enable )
	{
		m_enabled_log_types[t] = enable;
	}

	static TCHAR _static_format_buf[4096] = {0};
	
	nCoreAPI nstring	format_string(const TCHAR* fmt, ...)
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		return nstring(_static_format_buf);
	}

	void nlog::write( enum ELogType t, const TCHAR* txt)
	{
		if(txt == NULL)
			return;

		EnableLogTypeMap::const_iterator itEnabled = m_enabled_log_types.find( t );
		if (itEnabled != m_enabled_log_types.end() && !itEnabled->second)
			return;

		log_output_list::const_iterator itEnd = m_target_outputs.end();
		for (log_output_list::const_iterator it = m_target_outputs.begin(); it != itEnd; ++it)
		{
			(*it)->write_string(t, txt);
		}
	}

	void nlog::write_warning( const TCHAR* fmt,... )
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		write(ELog_Warning, _static_format_buf);
	}

	void nlog::write_info( const TCHAR* fmt,... )
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		write(ELog_Info, _static_format_buf);
	}

	void nlog::write_error( const TCHAR* fmt,... )
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		write(ELog_Error, _static_format_buf);
	}

	void nlog::write_exception( const TCHAR* fmt,... )
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		write(ELog_Exception, _static_format_buf);
	}

	void nlog::write_exception( const char* std_exp )
	{
		write(ELog_Exception, conv_string(std_exp).c_str());
	}

	void nlog::write_debug( const TCHAR* fmt,... )
	{
		va_list argptr;

		va_start(argptr, fmt);
		_vstprintf(_static_format_buf, fmt, argptr);
		va_end(argptr);

		write(ELog_Debug, _static_format_buf);
	}

	nfile_listener::nfile_listener( const nstring& fileName )
	{
		m_file.open(fileName.c_str(), ios_base::out);
		m_file << "// format: [logLevel] message \r\n";
		m_file.flush();
	}

	nfile_listener::~nfile_listener()
	{
		if( m_file.is_open() )
		{
			m_file.flush();
			m_file.close();
		}
	}

	void nfile_listener::write_string( enum ELogType t, const TCHAR* txt )
	{
		format_log_message(m_file, t, txt);
		m_file.flush();
	}

	log_output_ptr nfile_listener::add( const nstring& fileName )
	{
		log_output_ptr l( new nfile_listener(fileName) );
		nlog::instance()->add_log_output(l);
		return l;

	}


	nstderr_listener::nstderr_listener()
	{

	}

	nstderr_listener::~nstderr_listener()
	{

	}

	void nstderr_listener::write_string( enum ELogType t, const TCHAR* txt )
	{
		std::wostringstream ss;
		format_log_message(ss, t, txt);

		std::wcerr << ss.str();
	}

	nexus::log_output_ptr nstderr_listener::add()
	{
		log_output_ptr l( new nstderr_listener() );
		nlog::instance()->add_log_output( l );
		return l;
	}

	nconsole_listener::nconsole_listener()
	{

	}

	nconsole_listener::~nconsole_listener()
	{

	}

	void nconsole_listener::write_string( enum ELogType t, const TCHAR* txt )
	{
		AllocConsole(); // this would actually open a console window...

		if(txt == NULL)
			return;

		// save settings
		HANDLE outHandle = GetStdHandle( STD_ERROR_HANDLE );
		if (!outHandle)
			return;
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
		::GetConsoleScreenBufferInfo( outHandle, &csbiInfo );

		// print
		WORD wAttributes = FOREGROUND_INTENSITY;
		if (t == ELog_Error || t == ELog_Exception)
			wAttributes |= FOREGROUND_RED; // red
		else if (t == ELog_Warning)
			wAttributes |= FOREGROUND_GREEN | FOREGROUND_RED; // yellow
		else if (t == ELog_Debug)
			wAttributes |= FOREGROUND_GREEN; // green
		else
			wAttributes = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
		::SetConsoleTextAttribute( outHandle, wAttributes );
		DWORD dwWritten = 0;

		::BOOL bSuccess = WriteConsoleW( outHandle, txt, wcslen(txt), &dwWritten, 0 );
		assert( bSuccess );

		// restore settings
		::SetConsoleTextAttribute( outHandle, csbiInfo.wAttributes );
	}

	nexus::log_output_ptr nconsole_listener::add()
	{
		log_output_ptr l( new nconsole_listener() );
		nlog::instance()->add_log_output( l );
		return l;
	}



}//namespace nexus