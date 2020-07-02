#ifndef _NSERVER_H_
#define _NSERVER_H_

#include "nnetwork.h"

/**
 *	回响测试服务器
 */
namespace nexus {

	class nserver : public ntcp_server
	{
	public:
		nserver();
		virtual ~nserver();

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

		virtual void on_connect(uint32 session_id, uint32 address, uint32 port);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

	private:
		nsafe_mem_pool			m_pool;
	};

	extern nserver g_nserver;

} // namespace nexus

#endif //_NSERVER_H_