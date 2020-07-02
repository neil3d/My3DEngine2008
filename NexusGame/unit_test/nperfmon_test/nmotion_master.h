/**
 *	nexus nperfmon_test - nmotion_master
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NMOTION_MASTER_H_
#define _NPERFMON_TEST_NMOTION_MASTER_H_

#include "ncommon.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nlocal_player;

	class nmotion_master
	{
	public:
		nmotion_master();
		virtual ~nmotion_master();

		bool init(nlocal_player* player_ptr);
		void destroy();
		void update(uint32 elapse);

	private:
		int32 _rand(int32 min, int32 max);
		void _send_move_msg();
		void _send_attack_msg();
		void _cancel_attack_msg();

	private:
		nlocal_player*		m_player_ptr;
		bool				m_moving;
		bool				m_attacking;
		uint32				m_time_stamp;
		uint32				m_elapse;
		vector3				m_acceleration;
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NMOTION_MASTER_H_
