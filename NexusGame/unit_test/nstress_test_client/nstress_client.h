#ifndef _NSTESS_CLIENT_H_
#define _NSTESS_CLIENT_H_

#include "nnetwork.h"

namespace nexus {

	/**
	 *	—π¡¶≤‚ ‘øÕªß∂À
	 */
	class nstress_client : public ntcp_client_iocp
	{
	public:
		nstress_client();
		virtual ~nstress_client();

		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

		bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size);

	private:
		nsafe_mem_pool		m_pool;
	};

	extern nstress_client g_nstress_client;

} // namespace nexus

#endif // _NSTESS_CLIENT_H_
