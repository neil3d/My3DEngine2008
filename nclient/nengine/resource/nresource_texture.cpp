#include "StdAfx.h"
#include "nresource_texture.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"

namespace nexus
{
	nresource_texture_2d::nresource_texture_2d(const nstring& name_str):nresource_texture(name_str)
	{
	}

	nresource_texture_2d::~nresource_texture_2d(void)
	{
	}

	void nresource_texture_2d::load_from_file(const resource_location& loc)
	{
		m_location = loc;
		
		//--
		nfile_system* fs = nengine::instance()->get_file_sys();
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
	
		// for exception safe
		render_res_ptr<nrender_texture2D>	render_tex(rres_mgr->alloc_texture_2d());
		render_tex->load_from_file(fs, loc.pkg_name, loc.file_name);

		m_render_tex.reset(render_tex.release());
	}
}//namespace nexus