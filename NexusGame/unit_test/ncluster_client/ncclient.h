#ifndef _NCLIENT_H_
#define _NCLIENT_H_

#include "nnetwork.h"

/**
 *	ªÿœÏ≤‚ ‘øÕªß∂À
 */
namespace nexus {

#ifdef _NO_IOCP
#	define _cluster_client ncluster_client
#else
#	define _cluster_client nc_client
#endif

	class ncclient : public _cluster_client
	{
	public:
		ncclient();
		virtual ~ncclient();

		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

	private:
		nsafe_mem_pool			m_pool;
	};

	extern ncclient g_ncclient;

} // namespace nexus

#endif // _NCLIENT_H_
