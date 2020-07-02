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
#include <fstream>

namespace nexus
{
	enum ELogType
	{
		ELog_Info		= 0,
		ELog_Warning,
		ELog_Error,
		ELog_Exception,
		ELog_Debug,

		ELog_MAX
	};

	nCoreAPI void format_log_message(std::wostream& out, enum ELogType type, const TCHAR* msg);
	
	//-- Log输出接口
	struct nCoreAPI nlog_output
	{
		nlog_output() {}
		virtual ~nlog_output(void);

		virtual void write_string(enum ELogType t, const TCHAR* txt) = 0;

		// 禁止使用拷贝极造函数和赋值操作
		DISALLOW_COPY_AND_ASSIGN(nlog_output)
	};
	typedef boost::shared_ptr<nlog_output> log_output_ptr;

	struct nCoreAPI nfile_listener : public nlog_output
	{
		nfile_listener(const nstring& fileName);
		virtual ~nfile_listener();

		virtual void write_string(enum ELogType t, const TCHAR* txt);

		static log_output_ptr add(const nstring& fileName);
	private:
		std::wofstream	m_file;
	};

	struct nCoreAPI nstderr_listener : public nlog_output
	{
		nstderr_listener();
		virtual ~nstderr_listener();

		virtual void write_string(enum ELogType t, const TCHAR* txt);

		static log_output_ptr add();
	};

	/** 输出LOG信息到控制台 */
	struct nCoreAPI nconsole_listener : public nlog_output
	{
		nconsole_listener();
		virtual ~nconsole_listener();

		virtual void write_string(enum ELogType t, const TCHAR* txt);

		static log_output_ptr add();
	};

	//--
	class nCoreAPI nlog 
		: public singleton<nlog>
	{
	public:
		~nlog(void);

		void add_log_output(log_output_ptr new_output);
		void remove_log_output(log_output_ptr output);
		void clear_log_output();

		void enable_log_type(const ELogType t, const bool enable);

		void write(enum ELogType t, const TCHAR* txt);

		void write_info(const TCHAR* fmt,...);
		void write_warning(const TCHAR* fmt,...);
		void write_error(const TCHAR* fmt,...);
		void write_exception(const TCHAR* fmt,...);
		void write_exception(const char* std_exp);
		void write_debug(const TCHAR* fmt,...);

	private:
		typedef std::vector<log_output_ptr> log_output_list;
		log_output_list					m_target_outputs;

		typedef std::map<ELogType,bool> EnableLogTypeMap;
		EnableLogTypeMap				m_enabled_log_types;
	};
	

#define NEXUS_LOG_X_BASE( LOGTYPE, WHAT ) nlog::instance()->write( LOGTYPE, WHAT);

#define nLog_Append nlog::instance()->write_info
#define nLog_Info nlog::instance()->write_info
#define nLog_Warning nlog::instance()->write_warning
#define nLog_Error nlog::instance()->write_error
#define nLog_Exception nlog::instance()->write_exception
#define nLog_Debug nlog::instance()->write_debug

	nstring	nCoreAPI format_string(const TCHAR* fmt, ...);

#ifdef _DEBUG
	#define nTRACE nLog_Debug
#else
	#define nTRACE (void)
#endif

}//namespace nexus
#endif//_NEXUS_LOG_H_