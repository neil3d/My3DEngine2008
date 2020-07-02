/**
 *	nexus db - ncmd_queue
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCMD_QUEUE_H_
#define _NDB_NCMD_QUEUE_H_

#include "nmutex.h"
#include "nguard.h"

namespace nexus{

	#ifdef _UNICODE
		#define tcout wcout
		#define tcin wcin
	#else
		#define tcout cout
		#define tcin cin
	#endif

	class ncmd_queue
	{
	private:
		typedef std::list<tstring>		CmdStrList;
		typedef	CmdStrList::iterator	CmdStrListIt;

	public:
		ncmd_queue();
		~ncmd_queue();

		// 进入队列
		void enqueue(const nchar* cmd);

		// 出列
		tstring dequeue(void);

		bool empty(void);

	private:
		nmutex		m_mutex;
		CmdStrList	m_cmd_list;
	};
}

#endif _NDB_NCMD_QUEUE_H_