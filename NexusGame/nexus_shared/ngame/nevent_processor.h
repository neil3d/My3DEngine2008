/**
*	nexus ngame - nevent_processor
*
*	Copyright (C) 2010 NexusDev
*
*	Author: D. Zhao
*	Date:	Jun, 2010
*/

#ifndef _NGAME_NEVENT_PROCESSOR_H_
#define _NGAME_NEVENT_PROCESSOR_H_

#include "ncommon.h"
#include "nevent_base_args.h"
#include "nsafe_mem_pool.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	/**
	 *	线程安全说明, ts为线程安全函数
	 *	常用方式是基于nunit发送处理事件, 全局事件采用ts方式调用(可能在地图线程内调用), 全局事件update通过串行保证线程安全
	 *	note: 注意bind类的成员时, 要保证回调的类对象的生命期, 释放时先清空事件再析构各个模块
	 */
	class nevent_processor : private nnoncopyable
	{
	public:
		typedef boost::function<void (const nevent_base_args*)> nevent_handler;

	public:
		nevent_processor();
		~nevent_processor();

		/**
		 *	处理投递事件
		 */
		void update(uint32 elapse);

		/**
		 *	清空投递的事件
		 */
		void clear_events();

		/**
		 *	注册事件处理函数, 各个模块可以在自己的init中注册自己关心的事件，尽量保证注册事件处理函数和处理函数在同一个模块里
		 *	note:	与session的注册不同, 这里事件处理函数, 可以是不同类的对象, 这样注册最好在不同类中
		 *			session处理函数是同一个类的函数, 可以统一提出来管理
		 */
		void register_event_handler(uint16 event_id, nevent_handler handler);

		/**
		 *	直接触发事件, 发送事件者不关心事件具体由谁处理, 怎么处理, 处理结果
		 *	note: 函数调用, 调用者知道要发生什么事情，关心返回值
		 */
		void send_event(const void* event_ptr, uint32 size);

		/**
		 *	投递事件, 投递事件者不关心事件具体由谁处理, 怎么处理
		 */
		void post_event(uint32 expire_time, const void* data_ptr, uint32 size);

		/**
		 *	线程安全接口
		 */
		void send_event_ts(const void* event_ptr, uint32 size);
		void post_event_ts(uint32 expire_time, const void* data_ptr, uint32 size);

	private:
		struct nevent
		{
			uint32			expire_time;
			uint32			size;
			char			buffer[sizeof(uint32)];

			nevent() : expire_time(0), size(0) {}
		};

		typedef std::multimap<uint16, nevent_handler>	nevent_handler_map;
		typedef std::list<nevent*>						nevent_list;

	private:
		nevent_list							m_events;
		nevent_handler_map					m_handers;
		nfast_mutex							m_mutex;
		static nsafe_mem_pool				m_pool;
	};

} // namespace nexus

#endif // _NGAME_NEVENT_PROCESSOR_H_
