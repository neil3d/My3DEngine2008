#include "nlocal_player_mgr.h"
#include "nplayer.h"

namespace nexus {

	nlocal_player_mgr::nlocal_player_mgr()
	{
	}

	nlocal_player_mgr::~nlocal_player_mgr()
	{
	}

	bool nlocal_player_mgr::add_local_player(nlocal_player* player_ptr)
	{
		return true;
	}

	void nlocal_player_mgr::remove_local_player(nlocal_player* player_ptr)
	{

	}

	nlocal_player* nlocal_player_mgr::get_local_player(uint64 guid) const
	{
		return NULL;
	}

} // namespace nexus
