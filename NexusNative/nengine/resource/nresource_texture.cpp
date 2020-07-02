#include "StdAfx.h"
#include "nresource_texture.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif
namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nresource_texture, nresource)
	nDEFINE_NAMED_CLASS(nresource_texture_2d, nresource_texture)
	nDEFINE_NAMED_CLASS(nresource_cube_map, nresource_texture)

	//////////////////////////////////////////////////////////////////////////
	// class nresource_texture_2d
	//////////////////////////////////////////////////////////////////////////
	nresource_texture_2d::nresource_texture_2d(const nstring& name_str):nresource_texture(name_str)
	{
	}

	nresource_texture_2d::~nresource_texture_2d(void)
	{
	}

	void nresource_texture_2d::load_from_file(const resource_location& loc)
	{
		boost::mutex::scoped_lock lock(m_mutex);

		m_location = loc;
		
		//--
		nfile_system* fs = nengine::instance()->get_file_sys();
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
	
		// for exception safe
		render_res_ptr<nrender_texture2D>	render_tex(rres_mgr->alloc_texture_2d());
		render_tex->load_from_file(fs, loc.pkg_name, loc.file_name);

		m_render_tex.reset(render_tex.release());

		::InterlockedExchange(&m_ready, 1L);
	}

	//////////////////////////////////////////////////////////////////////////
	// class nresource_cube_map
	//////////////////////////////////////////////////////////////////////////
	nresource_cube_map::nresource_cube_map(const nstring& name_str):nresource_texture(name_str)
	{
	}

	nresource_cube_map::~nresource_cube_map(void)
	{
	}

	void nresource_cube_map::load_from_file(const resource_location& loc)
	{
		boost::mutex::scoped_lock lock(m_mutex);

		m_location = loc;

		//--
		nfile_system* fs = nengine::instance()->get_file_sys();
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		// for exception safe
		render_res_ptr<nrender_cube_map> render_tex(rres_mgr->alloc_cube_map());
		render_tex->load_from_file(fs, loc.pkg_name, loc.file_name);

		m_render_tex.reset(render_tex.release());

		::InterlockedExchange(&m_ready, 1L);
	}
}//namespace nexus