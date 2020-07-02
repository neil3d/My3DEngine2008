#ifndef _NSESSION_H_
#define _NSESSION_H_

#include "ntype_def.h"
#include "nnoncopyable.h"

namespace nexus {

	struct nmessage;
	struct nmsg_base;

	class nsession : private nnoncopyable
	{
	public:
		nsession();
		virtual ~nsession();

		bool init(uint32 session_id);
		void destroy();

		bool is_in_used() const { return 0XFFFFFFFF != m_session_id; }
		void set_sesssion_id(uint32 session_id) { m_session_id = session_id; }
		uint32 get_sesssion_id() const { return m_session_id; }

		bool send_message(nmessage* msg_ptr, uint32 size);
		bool check_message(const nmsg_base* packet);

	private:
		uint32			m_session_id;
		uint32			m_packet_id;
	};

} // namespace nexus

#endif //_NSESSION_H_
