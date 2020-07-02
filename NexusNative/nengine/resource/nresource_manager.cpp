#include "StdAfx.h"
#include <boost/bind.hpp>
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


	nresource_skeletal_anim_set::ptr nresource_manager::load_skeletal_anim_set( const resource_location& loc, enum EResourceIOMode mode/*=EIO_Auto*/, enum EResourceIOPriority pri/*=EIOP_Normal*/ )
	{
		return load_resource_T<nresource_skeletal_anim_set>(loc, mode, pri);
	}

	nresource_skeletal_mesh::ptr nresource_manager::load_skeletal_mesh( const resource_location& loc, enum EResourceIOMode mode/*=EIO_Auto*/, enum EResourceIOPriority pri/*=EIOP_Normal*/ )
	{
		return load_resource_T<nresource_skeletal_mesh>(loc, mode, pri);
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

	nresource_special_effect::ptr nresource_manager::new_special_effect(const nstring& res_name)
	{
		return create_empty_resource_T<nresource_special_effect>(res_name);
	}

	nmtl_tech_shader::ptr nresource_manager::load_mtl_shader(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nmtl_tech_shader_hlsl>(loc, mode, pri);		
	}



	nresource_special_effect::ptr nresource_manager::load_special_effect(const resource_location& loc, 
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_special_effect>(loc, mode, pri);		
	}

	void nresource_manager::close()
	{
		m_white_texture.reset();
		m_io->shutdown();
	}

	void nresource_manager::init(const nstring& cache_class, const nstring& io_class)
	{
		//--
		m_cache.reset( nconstruct<nresource_cache>( cache_class) );			
		m_io.reset( nconstruct<nresource_io>( io_class) );
		m_io->start();

		//-- default material
		nstring pkg_name = nengine::instance()->get_engine_package_name();

		m_default_mtl.create_from_hlsl(_T("AllTech"), resource_location(pkg_name,_T("material/ENG_Default.hlsl")) );
		
		m_default_mtl_two_sided.create_from_hlsl(_T("basic_pass"), resource_location(pkg_name,_T("material/ENG_Default.hlsl")) );

		//-- 使用一个纯白的贴图作为默认贴图
		m_white_texture = load_texture_2d(resource_location(pkg_name, _T("resource_default/white.bmp")), EIO_Block);

		//--
		nengine::instance()->get_renderer()->register_device_handler(
			boost::bind(&nresource_manager::on_device_lost, this, _1),
			boost::bind(&nresource_manager::on_device_reset, this, _1) );
	}

	nresource_texture_2d::ptr nresource_manager::load_texture_2d(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_texture_2d>(loc, mode, pri);
	}

	nresource_cube_map::ptr nresource_manager::load_cube_map(const resource_location& loc,
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_cube_map>(loc, mode, pri);
	}

	nresource_texture_atlas::ptr nresource_manager::load_texture_atlas(const resource_location& loc, 
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		return load_resource_T<nresource_texture_atlas>(loc, mode, pri);
	}

	template<typename T>
	boost::intrusive_ptr<T> nresource_manager::load_resource_T(const resource_location& loc,enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		nname rn(loc.to_string());

		//-- find in cache
		nresource* res_exist = m_cache->active_resource(rn);
		if( res_exist )
		{
			return boost::intrusive_ptr<T>((T*)res_exist, true);
		}

		//-- make new one
		boost::intrusive_ptr<T> new_res( nNew T(rn.name_str) );
		m_io->load_resource(new_res, loc.pkg_name, loc.file_name, mode, pri);
		m_cache->cache_resource(new_res.get());

		return new_res;
	}

	nspeed_base_tree::ptr nresource_manager::load_speed_tree(const resource_location& loc)
	{
		return load_resource_T<nspeed_base_tree>(loc, EIO_Block, EIOP_Normal);
	}



	void nresource_manager::on_device_lost(int param)
	{
		class resource_handler : public nresource_cache_visitor
		{
		public:
			virtual void handle_resource(nresource* res, int param)
			{
				res->_on_device_lost(param);
			}
		};

		resource_handler h;
		m_cache->accept_visitor(&h, param);
	}

	bool nresource_manager::on_device_reset(int param)
	{
		class resource_handler : public nresource_cache_visitor
		{
		public:
			virtual void handle_resource(nresource* res, int param)
			{
				bool ret = res->_on_device_reset(param);
				if (!ret)
				{
					nthrow(_T("resource device reset failed!"));
				}
			}
		};

		try
		{
			resource_handler h;
			m_cache->accept_visitor(&h, param);
		}
		catch(nexception& ne)
		{
			(void)ne;
			return false;
		}
		return true;
	}

	void nresource_manager::register_event_target(nevent_target* obj)
	{
		m_event_targets.insert(obj);
	}

	void nresource_manager::unregister_event_target(nevent_target* obj)
	{
		m_event_targets.erase(obj);
	}

	void nresource_manager::post_event(const nresource_event& evt)
	{
		m_events.push(evt);
	}

	void nresource_manager::dispatch_events()
	{
		nresource_event evt;
		while( !m_events.empty() )
		{
			m_events.pop( evt );

			for (std::set<nevent_target*>::iterator iter=m_event_targets.begin();
				iter != m_event_targets.end();
				++iter)
			{
				(*iter)->on_event(evt);
			}// end of for()
		}// end of while()
	}

	void nresource_event_queue::push(const nresource_event& evt)
	{
		boost::mutex::scoped_lock lock( m_mutex );
		m_events.push(evt);
	}

	bool nresource_event_queue::empty() const
	{
		boost::mutex::scoped_lock lock( m_mutex );
		return m_events.empty();
	}

	void nresource_event_queue::pop(nresource_event& out_evt)
	{
		boost::mutex::scoped_lock lock( m_mutex );
		out_evt = m_events.front();
		m_events.pop();
	}
}//namespace nexus