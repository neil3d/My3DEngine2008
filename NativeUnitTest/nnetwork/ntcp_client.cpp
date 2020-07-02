#include "ntcp_client.h"
#include "../ncore/ncore.h"

namespace nexus
{
	struct ntcp_client::impl
	{
		boost::shared_ptr<nasio_wrap> m_asio_ptr;	// asio::io_service的包装，避免向外部暴露asio
		int				m_conn_index;				// 连接流水号，用作connection的id

		impl(boost::shared_ptr<nasio_wrap> asio_ptr)
			: m_asio_ptr(asio_ptr), m_conn_index(0)
		{}
	};

	ntcp_client::ntcp_client(boost::shared_ptr<nasio_wrap> asio_ptr)
		: m_impl(new impl(asio_ptr))
	{
	}

	ntcp_client::~ntcp_client(void)
	{
	}

	ntcp_client_connection::ptr ntcp_client::create_connection()
	{
		return ntcp_client_connection::ptr(
			new ntcp_client_connection(m_impl->m_asio_ptr, 
			m_impl->m_conn_index++,NNET_MAX_LAN_PACKET_SIZE)
			);
	}
}// namespace nexus