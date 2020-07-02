#include "StdAfx.h"
#include "nactor_component.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nactor_component, nobject)

	nactor_component::nactor_component(const nstring& name_str):
		m_owner(NULL),m_name(name_str)
	{
	}

	nactor_component::~nactor_component(void)
	{
	}

	void nactor_component::_init(nactor* owner)
	{
		m_owner = owner;		
	}
}//namespace nexus