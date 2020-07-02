#include "StdAfx.h"
#include "sky_base.h"
#include "../framework/world_define.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "nsky_box.h"
#include "ndynamic_sky_dome.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nsky_actor, nactor)
	nDEFINE_VIRTUAL_CLASS(nsky_component, nprimitive_component)

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

	void nsky_actor::create_sky_box(const resource_location& cube_map_loc)
	{
		boost::shared_ptr<nsky_box> box = create_component_t<nsky_box>(_T("sky_box_comp"));
		box->create(cube_map_loc);
	}

	boost::shared_ptr<ndynamic_sky_dome> nsky_actor::create_sky_dome(float radius, int tesselation)
	{
		ndynamic_sky_dome::ptr dome = create_component_t<ndynamic_sky_dome>(_T("sky_dome_comp"));
		dome->create(radius, tesselation);		

		return dome;
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nsky_component
	////////////////////////////////////////////////////////////////////////////
	nsky_component::nsky_component(const nstring& name_str):nprimitive_component(name_str)
	{
		m_trans_flag.set(ETrans_Opaque);
		m_render_obj.init( this );
	}

	nsky_component::~nsky_component(void)
	{
	}

	void nsky_component::_init(nactor* owner)
	{
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);

		nprimitive_component::_init(owner);
	}

	void nsky_component::render(const nviewport& view)
	{
		(void)view;
		nrenderer_base* rnd = nengine::instance()->get_renderer();
		rnd->frame_object(&m_render_obj);
	}

	void nsky_component::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		nSERIALIZE(ar, m_trans_flag);
	}
}//namespace nexus