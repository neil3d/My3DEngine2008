#ifndef _NCLIENT_H_
#define _NCLIENT_H_

#include "nnetwork.h"

/**
 *	ªÿœÏ≤‚ ‘øÕªß∂À
 */
namespace nexus {

	class nclient : public ntcp_client
	{
	public:
		nclient();
		virtual ~nclient();

		virtual void on_connect(uint32 error_code);
		virtual void on_datarecv(const void* msg_ptr, uint32 size);
		virtual void on_disconnect();
	};

	extern nclient g_nclient;
	extern bool g_connected;

} // namespace nexus

#endif // _NCLIENT_H_
