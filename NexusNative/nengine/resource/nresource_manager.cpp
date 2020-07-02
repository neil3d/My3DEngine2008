#include "StdAfx.h"
#include "nresource_cache_basic.h"
#include "nresource_io_basic.h"

#include "nresource_manager.h"
#include "../framework/nengine.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nresource_cache, nobject)
	nDEFINE_VIRTUAL_CLASS(nresource_io, nobject)
	///////////////////////////////////////////////////////////////////////////////////////////
	//	class nresource_manager
	///////////////////////////////////////////////////////////////////////////////////////////
	nresource_manager::nresource_manager(void):m_default_mtl(_T("default_mtl")),
		m_default_mtl_two_sided(_T("default_mtl_two_sided"))
	{
	}

	nresource_manager::~nresource_manager(void)
	{
	}

	nresource_manager* nresource_manager::instance()
	{
		static nresource_manager g_res_mgr;
		return &g_res_mgr;
	}

	void nresource_manager::free_resource(nresource* res)
	{
		if( m_cache->release_resource(res) )
		{
			m_io->cancel_resource_load(res);
			delete res;
		}
	}

	nresource_static_mesh::ptr nresource_manager::load_static_mesh(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_static_mesh>(loc, mode, pri);
	}

	nresource_anim_mesh::ptr nresource_manager::load_anim_mesh(const resource_location& loc, 
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_anim_mesh>(loc, mode, pri);
	}

	nresource::ptr nresource_manager::find_resource_by_name(const nstring& res_name)
	{
		return nresource::ptr(m_cache->find_resource( nname(res_name) ),
			true);
	}

	template<typename T>
	boost::intrusive_ptr<T> nresource_manager::create_empty_resource_T(const nstring& res_name)
	{
		nASSERT(find_resource_by_name(res_name) == NULL);

		boost::intrusive_ptr<T> new_res( nNew T(res_name) );
		m_cache->cache_resource(new_res.get());

		return new_res;
	}

	nresource_static_mesh::ptr nresource_manager::new_static_mesh( const nstring& res_name )
	{
		return create_empty_resource_T<nresource_static_mesh>(res_name);
	}

	nresource_anim_mesh::ptr nresource_manager::new_anim_mesh( const nstring& res_name )
	{
		return create_empty_resource_T<nresource_anim_mesh>(res_name);
	}

	nresource_skeletal_mesh::ptr nresource_manager::new_skeletal_mesh(const nstring& res_name)
	{
		return create_empty_resource_T<nresource_skeletal_mesh>(res_name);
	}

	nresource_skeletal_anim_set::ptr nresource_manager::new_skeletal_anim(const nstring& res_name)
	{
		return create_empty_resource_T<nresource_skeletal_anim_set>(res_name);
	}

	nmaterial_template::ptr nresource_manager::load_material_template_script(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nmaterial_script>(loc, mode, pri);		
	}

	void nresource_manager::init()
	{
		//--
		m_cache.reset(nNew nresource_cache_basic);
		m_io.reset(nNew nresource_io_basic);

		//-- default material
		nstring pkg_name = nengine::instance()->get_engine_package_name();

		{
			nmaterial_template::ptr mtl_tmp = load_material_template_script(
				resource_location(pkg_name,_T("material/ENG_Default.hlsl"))
				);

			m_default_mtl.create(mtl_tmp);
		}

		{
			nmaterial_template::ptr mtl_tmp = load_material_template_script(
				resource_location(pkg_name,_T("material/ENG_DefaultTwoSided.hlsl"))
				);

			m_default_mtl_two_sided.create(mtl_tmp);
		}

		nengine::instance()->get_render_res_mgr()->set_default_material(&m_default_mtl, &m_default_mtl_two_sided);
	}

	nresource_texture::ptr nresource_manager::load_texture_2d(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_texture_2d>(loc, mode, pri);
	}

	template<typename T>
	boost::intrusive_ptr<T> nresource_manager::load_resource_T(const resource_location& loc,enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		nfile_system* fs = nengine::instance()->get_file_sys();		

		nname rn(loc.to_string());

		//-- find in cache
		nresource* res_exist = m_cache->active_resource(rn);
		if( res_exist )
		{
			return boost::intrusive_ptr<T>((T*)res_exist, true);
		}

		//-- make new one
		boost::intrusive_ptr<T> new_res( nNew T(rn.name_str) );
		
		m_io->load_resource(new_res.get(), fs, loc.pkg_name, loc.file_name, mode, pri);
		m_cache->cache_resource(new_res.get());

		return new_res;
	}

	nspeed_base_tree::ptr nresource_manager::load_speed_tree(const resource_location& loc)
	{
		return load_resource_T<nspeed_base_tree>(loc, EIO_Block, EIOP_Normal);
	}
}//namespace nexus