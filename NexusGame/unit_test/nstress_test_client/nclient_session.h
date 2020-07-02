#ifndef _NCLIENT_SESSION_H_
#define _NCLIENT_SESSION_H_

#include "ntype_def.h"
#include "nnoncopyable.h"

namespace nexus {

	struct nmessage;

	class nclient_session : private nnoncopyable
	{
	public:
		nclient_session();
		virtual ~nclient_session();

		bool init(uint32 session_id);
		void destroy();

		bool is_in_used() const { return 0XFFFFFFFF != m_session_id; }
		void set_sesssion_id(uint32 session_id) { m_session_id = session_id; }
		uint32 get_sesssion_id() const { return m_session_id; }

		void set_connected(bool connected) { m_connected = connected; }
		bool is_connected() const { return m_connected; }

		bool send_message(const void* msg_ptr, uint32 size);
		void send_move_test();

		void send_load_test();
		void send_cmd_test();

		bool check_recv_packet_id(uint32 packet_id);

	private:
		uint32			m_session_id;
		bool			m_connected;
		uint32			m_packet_id;

		uint32			m_recv_packet_id;
	};

} // namespace nexus

#endif //_NCLIENT_SESSION_H_
