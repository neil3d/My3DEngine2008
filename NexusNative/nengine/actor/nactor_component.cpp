#include "StdAfx.h"
#include "nactor_component.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nactor_component, nobject)

	nactor_component::nactor_component(const nstring& name_str):
		m_owner(NULL),m_name(name_str),m_editable(true)
	{
	}

	nactor_component::~nactor_component(void)
	{
	}

	void nactor_component::_init(nactor* owner)
	{
		m_owner = owner;		
	}

	void nactor_component::serialize(narchive& ar)
	{
		nevent_target::serialize(ar);

		nSERIALIZE(ar, m_name);
	}

	void nactor_component::_level_loaded(nactor* owner)
	{
		m_owner = owner;		
	}
}//namespace nexus