#ifndef _NCLIENT_SESSION_MGR_H_
#define _NCLIENT_SESSION_MGR_H_

#include "ntype_def.h"
#include "nnoncopyable.h"

namespace nexus {

	class nclient_session;

	class nclient_session_mgr : private nnoncopyable
	{
	public:
		nclient_session_mgr();
		virtual ~nclient_session_mgr();

		bool init(uint32 max_session);
		void destroy();

		nclient_session* add_session(uint32 session_id);
		void remove_session(uint32 session_id);
		nclient_session* get_session(uint32 session_id);

		// test
		void send_test();

	private:
		nclient_session*		m_session_ptrs;					// session指针容器
		uint32					m_max_session;					// 容纳的最大session数目
	};

	extern nclient_session_mgr g_nclient_session_mgr;

} // namespace nexus

#endif //_NCLIENT_SESSION_MGR_H_
