/**
 *	nexus ngame - nmsg_dispatch
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NMSG_DISPATCH_H_
#define _NGAME_NMSG_DISPATCH_H_

#include "ncommon.h"
#include "nmsg_base_id.h"

namespace nexus {

	struct nmsg_base;
	class nlan_session;
	class nclient_session;

	typedef void (nlan_session::*cluster_handler_type)(const nmsg_base* msg_ptr);
	typedef void (nclient_session::*client_handler_type)(const nmsg_base* msg_ptr);

	struct ncluster_msg_handler
	{
		tstring					msg_name;
		uint32					status;
		uint32					handle_count;
		cluster_handler_type	handler;

		ncluster_msg_handler() : msg_name(_T("")), status(0), handle_count(0), handler(NULL) {}
	};

	struct nclient_msg_handler
	{
		tstring					msg_name;
		uint32					status;
		uint32					handle_count;
		bool					map_task;				// ##@@位置和函数指针换的话，取不出正确数据。要么结构体定义放到单独文件
		client_handler_type		handler;

		nclient_msg_handler() : msg_name(_T("")), status(0), handle_count(0), map_task(false), handler(NULL) {}
	};

	/**
	 *	nmsg_dispatch
	 */
	class nmsg_dispatch
	{
	public:
		nmsg_dispatch();
		~nmsg_dispatch();

		bool init();
		void destroy();

		ncluster_msg_handler* get_cluster_msg_handler(uint16 msg_id);
		nclient_msg_handler* get_client_msg_handler(uint16 msg_id);

		bool is_map_task_msg(uint16 msg_id);

	private:
		void _register_cluster_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, cluster_handler_type handler);
		void _register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler, bool map_task = false);

	private:
		ncluster_msg_handler		m_cluster_msg_handler[Msg_Max];
		nclient_msg_handler			m_client_msg_handler[Msg_Max];
	};

	extern nmsg_dispatch g_msg_dispatch;

} // namespace nexus

#endif // _NGAME_NMSG_DISPATCH_H_
