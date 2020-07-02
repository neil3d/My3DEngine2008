#include "StdAfx.h"
#include "neditor_cmd_mgr.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(neditor_cmd, nobject)

	neditor_cmd_mgr::neditor_cmd_mgr(void)
	{
	}

	neditor_cmd_mgr::~neditor_cmd_mgr(void)
	{
	}

	void neditor_cmd_mgr::append_cmd(neditor_cmd::ptr cmd_ptr)
	{}

	neditor_cmd::ptr neditor_cmd_mgr::get_last_cmd() const
	{
		return neditor_cmd::ptr();
	}

	void neditor_cmd_mgr::undo() const
	{}

	void neditor_cmd_mgr::redo() const
	{}
}//namespace nexus