#include "lib_define.h"
#include <boost/asio.hpp>
#include "nsvr_net_io.h"
#include "nasio_wrap.h"
#include "../ncore/ncore.h"

namespace nexus
{

	nsvr_net_io::nsvr_net_io(void)
	{
	}

	nsvr_net_io::~nsvr_net_io(void)
	{
	}

	void nsvr_net_io::startup()
	{
		m_impl.reset(new nasio_wrap);
		m_impl->startup();
	}

	void nsvr_net_io::shutdown()
	{
		m_impl->shutdown();
	}

	ntcp_svr::ptr nsvr_net_io::create_tcp_svr()
	{
		return ntcp_svr::ptr(new ntcp_svr(m_impl));
	}

	ntcp_client::ptr nsvr_net_io::create_tcp_client()
	{
		return ntcp_client::ptr(new ntcp_client(m_impl));
	}
}//namespace nexus