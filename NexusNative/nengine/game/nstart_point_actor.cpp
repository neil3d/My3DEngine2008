#include "stdafx.h"
#include "nstart_point_actor.h"
#include "world/nlevel_info_resource.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nstart_point_actor, nactor)

	nstart_point_actor::nstart_point_actor( const nstring& name_str )
		: nactor(name_str)
	{
		m_data.name.reset(name_str);
	}

	nstart_point_actor::~nstart_point_actor( void )
	{

	}

	void nstart_point_actor::serialize( narchive& ar )
	{
		nactor::serialize(ar);

		nSERIALIZE(ar, m_data);
	}

	void nstart_point_actor::update_cmp_transform()
	{
		nactor::update_cmp_transform();

		m_data.location = m_space.location;
		m_data.rotation = m_space.rotation;
	}

	bool nstart_point_actor::export_info_resource( gameframework::nlevel_info_resource* res )
	{
		nactor::export_info_resource(res);
		
		nASSERT(res != NULL);
		nASSERT(get_name() == m_data.name);
		res->add_info(m_data);

		return true;
	}
} // end of namespace nexus