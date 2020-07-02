/**
 *	nexus ngame - nplayer_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NPLAYER_MGR_H_
#define _NGAME_NPLAYER_MGR_H_

#include "ncommon.h"

namespace nexus {

	class nplayer;
	struct nplayer_info;

	class nplayer_mgr
	{
	public:
		typedef boost::unordered_map<uint64, nplayer*>			PLAYER_MAP;
		typedef boost::unordered_map<uint64, nplayer_info*>		PLAYER_INFO_MAP;

	public:
		nplayer_mgr();
		virtual ~nplayer_mgr();

		bool add_player(nplayer* player_ptr);
		void remove_player(nplayer* player_ptr);
		nplayer* get_player(uint64 guid) const;

		nplayer* get_player_by_name_crc(uint32 name_crc) const;
		uint64 get_player_id(uint32 name_crc) const;
		const nchar* get_name(uint64 guid) const;

	private:
		PLAYER_MAP				m_player_map;
	};


} // namespace nexus

#endif // _NGAME_NPLAYER_MGR_H_
