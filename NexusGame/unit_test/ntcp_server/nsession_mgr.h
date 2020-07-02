#ifndef _NSESSION_MGR_H_
#define _NSESSION_MGR_H_

#include "ntype_def.h"
#include "nnoncopyable.h"

namespace nexus {

	class nsession;

	class nsession_mgr : private nnoncopyable
	{
	public:
		nsession_mgr();
		virtual ~nsession_mgr();

		bool init(uint32 max_session);
		void destroy();

		nsession* add_session(uint32 session_id);
		void remove_session(uint32 session_id);
		nsession* get_session(uint32 session_id);

		bool get_broadcast_seesion_id(uint32* sessions_ptr, uint32& num);

	private:
		nsession*				m_session_ptrs;					// session指针容器
		uint32					m_max_session;					// 容纳的最大session数目
	};

	extern nsession_mgr g_nsession_mgr;

} // namespace nexus

#endif //_NSESSION_MGR_H_
