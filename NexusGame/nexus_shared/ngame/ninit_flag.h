/**
 *	nexus ngame - ninit_flag
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NINIT_FLAG_H_
#define _NGAME_NINIT_FLAG_H_

#include "ncommon.h"

namespace nexus {

	enum EInitFlag
	{
		EIF_Loin		= 0x00000001,
		EIF_Bg			= 0x00000002,
		EIF_Db			= 0x00000004,

		// ...

	};

	class ninit_flag : private nnoncopyable
	{
	public:
		ninit_flag();
		virtual ~ninit_flag();

		void set_init_flag(uint32 flag);
		void gateway_login(uint32 session_id);
		bool is_init_ok() const { return 0 == m_flag; }
	
	private:
		void _send_gateway_login_info();

	private:
		uint32			m_flag;

		typedef std::vector<uint32> ID_VECTOR;
		ID_VECTOR		m_session_ids;
	};

	extern ninit_flag g_init_flag;

} // namespace nexus

#endif // _NGAME_NINIT_FLAG_H_
