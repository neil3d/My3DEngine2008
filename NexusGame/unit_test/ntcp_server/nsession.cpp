#include "nsession.h"
#include "nserver.h"
#include "echo_test.h"

#include <iostream>

namespace nexus {

	nsession::nsession() : m_session_id(0XFFFFFFFF), m_packet_id(0)
	{
	}

	nsession::~nsession()
	{
	}

	bool nsession::init(uint32 session_id)
	{
		m_session_id = session_id;
		m_packet_id = 0;
		return true;
	}

	void nsession::destroy()
	{
		m_session_id = 0XFFFFFFFF;
		m_packet_id = 0;
	}

	bool nsession::send_message(nmessage* msg_ptr, uint32 size)
	{
		if (!msg_ptr || 0 == size)
		{
			msg_ptr->dec_reference();
			return false;
		}

		return g_nserver.send(m_session_id, msg_ptr, size);
	}

	bool nsession::check_message(const nmsg_base* packet)
	{
		const tagNC_MoveTest* data_ptr = static_cast<const tagNC_MoveTest*>(packet);

		if (m_packet_id != data_ptr->flag)
		{
			return false;
		}

		++m_packet_id;
		return true;
	}

} // namespace nexus