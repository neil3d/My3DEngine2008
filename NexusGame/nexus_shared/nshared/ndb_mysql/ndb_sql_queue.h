/**
 *	nexus nshared - ndb_mysql
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_SQL_QUEUE_H_
#define _NSHARED_NDB_SQL_QUEUE_H_

#include "nmutex.h"
#include "ndb_utility.h"

namespace nexus{

	class ndb_query_stream;

	/**
	 *	ndb_sql_queue
	 */
	class ndb_sql_queue
	{
	private:
		typedef	std::list<ndb_query_stream*>	QueueList;
		typedef	QueueList::iterator				QueueListIt;

	public:
		ndb_sql_queue(void);
		~ndb_sql_queue(void);

		//sql语句进入队列
		void add(ndb_query_stream* sql_stream);

		// 从队列中取出sql语句
		ndb_query_stream* get(void);

		// 获取队列为空的事件
		const HANDLE& get_empty_event(void){ return m_empty_event; }

	private:
		// 获取队列sql语句的数量
		uint32 length(void){ return static_cast<uint32>(m_queue_one.size() + m_queue_other.size()); }

		bool empty(void){ return (0 == length()); }

	private:
		QueueList	m_queue_one;			//队列
		QueueList	m_queue_other;			//队列

		nmutex		m_mutex_read;			//读队列互斥信号量
		QueueList*	m_queue_read_ptr;		//当前读队列

		nmutex		m_mutex_write;			//写队列互斥信号量
		QueueList*	m_queue_wirte_ptr;		//当前写队列

		HANDLE		m_change_queue_event;	//读写队列交换事件
		uint32		m_wait_change_queue;	//等待读写事件的时间(单位：毫秒)

		HANDLE		m_empty_event;			//队列为空事件

	};
} // namespace nexus

#endif	_NSHARED_NDB_SQL_QUEUE_H_