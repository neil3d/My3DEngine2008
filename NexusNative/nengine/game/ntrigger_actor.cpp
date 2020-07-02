#include "stdafx.h"
#include "ntrigger_actor.h"
#include "world/nlevel_info_resource.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(ntrigger_actor, nactor)

	ntrigger_actor::ntrigger_actor( const nstring& name_str )
		: nactor(name_str)
	{
		m_data.name.reset(name_str);
	}

	ntrigger_actor::~ntrigger_actor( void )
	{

	}

	void ntrigger_actor::serialize( narchive& ar )
	{
		nactor::serialize(ar);

		nSERIALIZE(ar, m_data);
	}

	void ntrigger_actor::update_cmp_transform()
	{
		nactor::update_cmp_transform();
		m_data.location = m_space.location;
	}

	bool ntrigger_actor::export_info_resource( gameframework::nlevel_info_resource* res )
	{
		nactor::export_info_resource(res);

		nASSERT(res != NULL);
		nASSERT(get_name() == m_data.name);
		res->add_info(m_data);

		return true;
	}
} // end of namespace nexus