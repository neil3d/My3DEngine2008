#include "nplayer.h"
#include "nclient_session.h"
#include "nmap_mgr.h"
#include "nlog_mt.h"
#include "nmsg_char.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"

namespace nexus {

	nplayer::nplayer() : nunit()
	{
	}

	nplayer::~nplayer()
	{
	}

	bool nplayer::init()
	{
		return true;
	}

	void nplayer::destroy()
	{
		// TODO: D. Zhao add player destroy


		nunit::destroy();
	}

	void nplayer::update(uint32 elapse)
	{
		nunit::update(elapse);
		// TODO: D. Zhao add player update

	}


} // namespace nexus
