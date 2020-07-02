#include "nwan_session.h"

namespace nexus {

	nwan_session::nwan_session() : m_session_id(0XFFFFFFFF), m_status(EWSS_None), m_address(0), m_port(0)
	{
	}

	nwan_session::~nwan_session()
	{
	}

	bool nwan_session::init(uint32 session_id, uint32 address, uint16 port)
	{
		m_session_id	= session_id;
		m_status		= EWSS_Connected;
		m_address		= address;
		m_port			= port;
		return true;
	}

	void nwan_session::destroy()
	{
		m_session_id	= 0XFFFFFFFF;
		m_status		= EWSS_None;
		m_address		= 0;
		m_port			= 0;
	}

} // namespace nexus
