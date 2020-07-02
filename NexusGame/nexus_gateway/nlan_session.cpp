#include "nlan_session.h"
#include "nenums.h"

namespace nexus {

	nlan_session::nlan_session() : m_session_id(0XFFFFFFFF), m_server_type(EST_None), m_status(ELSS_None), m_address(0), m_port(0)
	{
	}

	nlan_session::~nlan_session()
	{
	}

	bool nlan_session::init(uint32 session_id, uint32 server_type, uint32 address, uint16 port)
	{
		m_session_id	= session_id;
		m_server_type	= server_type;
		m_status		= ELSS_Connected;
		m_address		= address;
		m_port			= port;
		return true;
	}

	// destroy 要进行该类所有数据的复原，因为要归还到池里
	void nlan_session::destroy()
	{
		m_session_id	= 0XFFFFFFFF;
		m_server_type	= EST_None;
		m_status		= ELSS_Closed;
		m_address		= 0;
		m_port			= 0;
	}

} // namespace nexus
