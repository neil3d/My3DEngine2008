#include "nlocal_player.h"
#include "nmap_mgr.h"
#include "nlog_mt.h"
#include "nmsg_char.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"

namespace nexus {

	nlocal_player::nlocal_player() : nplayer(), m_session_ptr(NULL)
	{
		m_controller_ptr.reset(new nlocal_cc);
	}

	nlocal_player::~nlocal_player()
	{
	}

	bool nlocal_player::init(nclient_session* session_ptr)
	{
		m_session_ptr = session_ptr;
		m_data.init(EPF_PlayerEnd);
		m_motion_master.init(this);
		return true;
	}

	void nlocal_player::destroy()
	{
		// TODO: D. Zhao add player destroy


		nunit::destroy();
	}

	void nlocal_player::update(uint32 elapse)
	{
		nplayer::update(elapse);
		// TODO: D. Zhao add player update

		m_motion_master.update(elapse);
	}

} // namespace nexus
