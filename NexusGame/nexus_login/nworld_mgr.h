/**
 *	nexus nlogin - nworld_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NWORLD_MGR_H_
#define _NLOGIN_NWORLD_MGR_H_

#include "ncommon.h"

namespace nexus {

	class nworld;

	class nworld_mgr : private nnoncopyable
	{
	public:
		typedef boost::unordered_map<uint32, nworld*>			WORLD_MAP;
		typedef boost::unordered_map<uint32, uint32>			CRC2ID_MAP;

	public:
		nworld_mgr();
		~nworld_mgr();

		bool init();
		void destroy();
		void update(uint32 elapse);

		bool add_world(nworld* world_ptr);
		void remove_world(nworld* world_ptr);
		nworld* get_world(uint32 world_id) const;
		nworld* get_world_by_name_crc(uint32 name_crc) const;

	private:
		WORLD_MAP					m_worlds;
		CRC2ID_MAP					m_crc_to_id;
	};

	extern nworld_mgr g_world_mgr;

} // namespace nexus

#endif // _NLOGIN_NWORLD_MGR_H_
