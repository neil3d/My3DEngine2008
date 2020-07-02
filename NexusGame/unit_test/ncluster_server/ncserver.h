#ifndef _NCSERVER_H_
#define _NCSERVER_H_

#include "nnetwork.h"

/**
 *	回响测试服务器
 */
namespace nexus {

#ifdef _NO_IOCP
#	define _cluster_server ncluster_server
#else
#	define _cluster_server nc_server
#endif

	class ncserver : public _cluster_server
	{
	public:
		ncserver();
		virtual ~ncserver();

		template <typename _type>
		_type* alloc_msg()
		{
			nmessage* msg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + sizeof(_type)));

			if (NULL == msg_ptr)
			{
				return NULL;
			}

			msg_ptr->pool_ptr	= &m_pool;
			msg_ptr->next_ptr	= NULL;
			msg_ptr->reference	= 1;
			msg_ptr->size		= sizeof(_type);
			msg_ptr->session_id	= 0;
			msg_ptr->server_id	= 0;

			// 调用构造函数初始化必要的数据
			new (&(msg_ptr->session_id)) _type();
			return reinterpret_cast<_type*>(&(msg_ptr->session_id));
		}

		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

	private:
		nsafe_mem_pool			m_pool;
	};

	extern ncserver g_ncserver;

} // namespace nexus

#endif //_NCSERVER_H_