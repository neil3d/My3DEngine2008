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

namespace nexus
{
	class nCoreAPI nlog 
		: public singleton<nlog>
	{
	public:
		nlog(void);
		~nlog(void);

		void init(const nstring& file_name);

		void write(const TCHAR* fmt,...);
		void write_info(const TCHAR* fmt,...);
		void write_warning(const TCHAR* fmt,...);
		void write_error(const TCHAR* fmt,...);
		void write_exception(const TCHAR* fmt,...);
		void write_exception(const char* std_exp);

		void trace(const TCHAR* fmt,...);
	private:
		struct impl;
		struct impl* m_impl;
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