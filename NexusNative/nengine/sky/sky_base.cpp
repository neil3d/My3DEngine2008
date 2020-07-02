#include "StdAfx.h"
#include "sky_base.h"
#include "../framework/world_define.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nsky_actor, nactor)
	nDEFINE_NAMED_CLASS(nsky_component, nstatic_mesh_component)

	////////////////////////////////////////////////////////////////////////////
	//	class nsky_actor
	////////////////////////////////////////////////////////////////////////////
	nsky_actor::nsky_actor(const nstring& name_str):nactor(name_str)
	{
		m_frozen = true;
	}

	nsky_actor::~nsky_actor(void)
	{
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nsky_component
	////////////////////////////////////////////////////////////////////////////
	nsky_component::nsky_component(const nstring& name_str):nstatic_mesh_component(name_str)
	{		
	}

	nsky_component::~nsky_component(void)
	{
	}

	void nsky_component::serialize( narchive& ar )
	{
		nstatic_mesh_component::serialize(ar);
		nSERIALIZE(ar,m_material);
		if(ar.is_loading())
		{
			nmtl_base::ptr mtl=nmtl_base::create_from_file(m_material);
			replace_material(0,0,mtl);
		}
	}
}//namespace nexus