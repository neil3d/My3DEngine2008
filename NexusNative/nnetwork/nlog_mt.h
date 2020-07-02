/**
 *	nexus network - nlog_mt
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NLOG_MT_H_
#define _NNETWORK_NLOG_MT_H_

#include "nsingleton.h"
#include "nmutex.h"
#include "nsafe_mem_pool.h"
#include "nunicode.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <string>
#include <fstream>

namespace nexus {

	enum ELogMTType
	{
		ELT_Normal	= 0,
		ELT_Fatal	= 1,
		ELT_NError	= 2,
		ELT_Error	= 3,
		ELT_Warning	= 4,
		ELT_Info	= 5,
		ELT_Debug	= 6,
		ELT_Max
	};

	/**
	 *	nlog_mt 异步的方式写入控制台或文件
	 */
	class nNET_API nlog_mt
	{
	private:
		struct nlog_msg
		{
			nlog_msg*	next_ptr;
			uint32		log_type;
			nchar		buffer[1];

			nlog_msg() : next_ptr(NULL), log_type(0) {}
		};

		enum EColorType
		{
			ECT_White		= 0,
			ECT_Red			= 1,
			ECT_Pink		= 2,
			ECT_Yellow		= 3,
			ECT_Blue		= 4,
			ECT_Green		= 5,
			ECT_Max
		};

	public:
		nlog_mt();
		~nlog_mt();

		/** 
		 * 初始化log, 每次启动log都会生成新的log文件，文件名称log_name + time + .log,  /log路径下
		 * @param log_name			该log对象的名称，会显示到相应console或文件里
		 * @param screen_log_level	console的log等级，ELogType <= screen_log_level 都可见
		 * @param file_log_level	file的log等级，ELogType <= file_log_level 都可见
		 * @param color				console log是否着色
		 * @param time				log里是否包含时间信息
		 */
		bool init(const tstring& log_name, uint32 screen_log_level, uint32 file_log_level, bool color = true, bool time = true);
		
		/** 
		 * 销毁log
		 */
		void destroy();

		void set_screen_log_level(uint32 level) { m_screen_log_level = level; }
		void set_file_log_level(uint32 level) { m_file_log_level = level; }

		void out_string(const nchar* str, ...);
		void out_fatal(const nchar* str, ...);
		void out_net_error(const nchar* str, ...);
		void out_error(const nchar* str, ...);
		void out_warning(const nchar* str, ...);
		void out_info(const nchar* str, ...);
		void out_debug(const nchar* str, ...);

	private:
		void _worker_thread();
		void _push_log_msg(nlog_msg* log_msg_ptr);
		void _handle_log_msg(nlog_msg* log_msg_ptr);
		tstring _get_timestamp();
		void _set_color(uint32 color);
		void _reset_color();

	private:
		tstring				m_log_name;
		nfast_mutex			m_mutex;
		tofstream			m_app_file;
		tofstream			m_net_file;

		nsafe_mem_pool		m_pool;

		boost::scoped_ptr<boost::thread>		m_worker_thread_ptr;
		volatile LONG		m_terminate;
		volatile LONG		m_log_msg;

		nlog_msg*			m_head_ptr;
		nlog_msg*			m_tail_ptr;

		uint32				m_screen_log_level;
		uint32				m_file_log_level;

		bool				m_colored;
		bool				m_include_time;

		HANDLE				m_stdout_handle;

		tstring				m_node_str[ELT_Max];
		uint32				m_color[ELT_Max];
		WORD				m_win_color_fg[ECT_Max];
	};

	#define NLOG			nsingleton<nlog_mt>::instance()
	#define NLOG_STRING		nsingleton<nlog_mt>::instance().out_string
	#define NLOG_FATAL		nsingleton<nlog_mt>::instance().out_fatal
	#define NLOG_NERROR		nsingleton<nlog_mt>::instance().out_net_error
	#define NLOG_ERROR		nsingleton<nlog_mt>::instance().out_error
	#define NLOG_WARNING	nsingleton<nlog_mt>::instance().out_warning
	#define NLOG_INFO		nsingleton<nlog_mt>::instance().out_info

	#ifdef _DEBUG
	#define NLOG_DEBUG		nsingleton<nlog_mt>::instance().out_debug
	#else
		#define NLOG_DEBUG
	#endif

} // namespace nexus

#endif _NNETWORK_NLOG_MT_H_
