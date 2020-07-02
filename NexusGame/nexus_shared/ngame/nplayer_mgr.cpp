#include "nplayer_mgr.h"
#include "nplayer.h"
#include "nplayer_define.h"

namespace nexus {

	nplayer_mgr::nplayer_mgr()
	{
	}

	nplayer_mgr::~nplayer_mgr()
	{
	}

	bool nplayer_mgr::add_player(nplayer* player_ptr)
	{
		assert(NULL != player_ptr);

		if (!m_player_map.insert(std::make_pair(player_ptr->get_id(), player_ptr)).second)
		{
			return false;
		}

		return true;
	}

	void nplayer_mgr::remove_player(nplayer* player_ptr)
	{
		assert(NULL != player_ptr);
		m_player_map.erase(player_ptr->get_id());
	}

	nplayer* nplayer_mgr::get_player(uint64 guid) const
	{
		PLAYER_MAP::const_iterator iter = m_player_map.find(guid);

		if (iter == m_player_map.end())
		{
		return NULL;
	}

		return iter->second;
	}

	nplayer* nplayer_mgr::get_player_by_name_crc(uint32 name_crc) const
	{
		return NULL;
	}

	uint64 nplayer_mgr::get_player_id(uint32 name_crc) const
	{
		return 0;
	}

	const nchar* nplayer_mgr::get_name(uint64 guid) const
	{
		return NULL;
	}

} // namespace nexus
