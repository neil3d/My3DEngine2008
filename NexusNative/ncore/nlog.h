/**
*	nexus core - Log
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_LOG_H_
#define _NEXUS_LOG_H_
#include "object/singleton.h"
#include "base_define.h"
#include "base_depend.h"
#include "object/nobject.h"

namespace nexus
{
	enum ELogType
	{
		ELog_Info,
		ELog_Warning,
		ELog_Error,
		ELog_Exception,
	};

	//-- LogÊä³ö½Ó¿Ú
	class nCoreAPI nlog_output : public nobject
	{
	public:
		nlog_output(void)	{}
		virtual ~nlog_output(void)	{}

		virtual void open(const nstring& param) = 0;
		virtual void write_string(const TCHAR* txt) = 0;
		virtual void write_string(enum ELogType t, const TCHAR* txt) = 0;
		virtual void close() = 0;

		nDECLARE_VIRTUAL_CLASS(nlog_output)
	};

	//--
	class nCoreAPI nlog 
		: public singleton<nlog>
	{
	public:
		nlog(void);
		~nlog(void);

		void init(const nstring& out_class_name, const nstring& param);
		void redirect(boost::shared_ptr<nlog_output> new_output);
		

		void write(const TCHAR* fmt,...);
		void write_info(const TCHAR* fmt,...);
		void write_warning(const TCHAR* fmt,...);
		void write_error(const TCHAR* fmt,...);
		void write_exception(const TCHAR* fmt,...);
		void write_exception(const char* std_exp);

		nstring get_last_exception();

		void trace(const TCHAR* fmt,...);
	private:
		boost::shared_ptr<nlog_output>	m_default_out;
		boost::shared_ptr<nlog_output>	m_out_device;		
		nstring							m_last_exception;
	};
	

#define nLog_Append nlog::instance()->write
#define nLog_Info nlog::instance()->write_info
#define nLog_Warning nlog::instance()->write_warning
#define nLog_Error nlog::instance()->write_error
#define nLog_Exception nlog::instance()->write_exception

	nstring	format_string(const TCHAR* fmt, ...);

#ifdef _DEBUG
	#define nTRACE nlog::instance()->trace
#else
	#define nTRACE (void)
#endif

}//namespace nexus
#endif//_NEXUS_LOG_H_