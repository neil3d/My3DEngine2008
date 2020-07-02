#include "StdAfx.h"
#include <boost/bind.hpp>
#include "nengine.h"
#include "nsubsystem.h"
#include "../resource/nresource_manager.h"
#include "../resource/resource_importer_manager.h"
#include "../image/nimage.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

namespace nexus
{
	//------------------------------------------------------------------------------------------------
	nDEFINE_VIRTUAL_CLASS(nsubsystem, nobject)
	nDEFINE_CLASS(nengine, nobject)

	nengine* nengine::s_engine_ptr = NULL;

	nengine::nengine(void)
	{
		nASSERT( s_engine_ptr == NULL);//只允许创建一个实例
		s_engine_ptr = this;		
	}

	nengine::~nengine(void)
	{
		s_engine_ptr = NULL;		
	}

	nengine* nengine::instance()
	{
		nASSERT(s_engine_ptr != NULL);
		return s_engine_ptr;
	}

	template<typename T>
	T* create_plugin_class_object(const nstring& class_name)
	{
		const nclass* sub_class = nclass_factory::instance()->find_class(class_name);
		if(sub_class == NULL)
			throw nexception(_T("plugin class not find: "), class_name.c_str(), _T("engine int failed!"));
		
		if( !sub_class->is_kind_of( T::reflection_get_static_class() ) )
			throw nexception(_T("bad plugin class name: "), class_name.c_str(), _T("engine int failed!"));

		return static_cast<T*>( sub_class->alloc_object() );
	}

	void nengine::init_core()
	{
		nexus_init_core();

		//nlog::instance()->init(_T("nlog_file"), _T("nexus.log"));
		//-- plugins
		load_plugins();

		//-- load dll会增加新的class，所以需要创建类继承关系
		nclass_factory::instance()->build_sub_classes();

		//-- resource importer, for editor only
		resource_importer_manager::instance()->load_plugin_classes();

		//-- init DevIL SDK
		nimage::init_image_sdk();
	}

	void nengine::init_modules(const engine_config& cfg)
	{
		m_config = cfg;
		if( m_config.file_sys_root.empty() )
			m_config.file_sys_root = nfile_sys_default_root();

		//-- file system
		m_file_sys.reset( create_plugin_class_object<nfile_system>(cfg.file_system_class) );
		m_file_sys->init(m_config.file_sys_root);
		m_file_sys->open_package(cfg.engine_data_pkg);

		//-- renderer
		m_renderer.reset(create_plugin_class_object<nrenderer_base>(cfg.renderer_class));
		m_renderer->set_file_system(m_file_sys, cfg.engine_data_pkg);
		m_renderer->init(m_config);

		//--
		m_renderer->register_device_handler(
			boost::bind(&nengine::on_device_lost, this, _1),
			boost::bind(&nengine::on_device_reset, this, _1) );

		//-- render resource manager
		nstring render_res_mgr_class_name = m_renderer->get_resource_manager_class_name();
		m_render_res_mgr.reset(create_plugin_class_object<nrender_resource_manager>(render_res_mgr_class_name));

		nresource_manager::instance()->init(cfg.resource_cache_class, cfg.resource_io_class);
		//--

		//-- create physics engine
		m_physics_engine.reset( nconstruct<nphysics_engine>( _T("nphysics_engine") ));
		m_physics_engine->init(cfg.phys_cfg);

		nLog_Info(_T("Nexus engine init succeeded!\r\n"));
	}

	extern HMODULE g_dll_handle;

	size_t nengine::load_plugins()
	{
		TCHAR szPath[1024]={0};
		::GetModuleFileName(g_dll_handle, szPath, sizeof(szPath));
		nstring szMdlPath(szPath);
		int f = szMdlPath.rfind(_T('\\'));
		szMdlPath = szMdlPath.substr(0, f);

		boost::filesystem::wpath base_path(szMdlPath);
		nLog_Info( _T("loading plugins from '%s'...\r\n"), base_path.string().c_str() );
		
		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(base_path);
			iter != end_iter;
			++iter)
		{
			if(boost::filesystem::is_regular( iter->status() ) )
			{
				boost::filesystem::wpath path = *iter;
				nstring path_str = path.string();

				if(boost::iends_with(path_str, ".nex"))
				{
					nshared_lib::ptr lib = nshared_lib::load_lib(path_str);
					m_plugin_array.push_back(lib);

					nLog_Info(_T("plugin loaded -- %s \r\n"), path.leaf().c_str());
				}
			}
		}

		return m_plugin_array.size();
	}

	void nengine::update(float delta_time,const nviewport& view)
	{
		nresource_manager::instance()->dispatch_events();
		m_physics_engine->update(delta_time);

		for(st_level_list::iterator iter = m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			(*iter)->update(delta_time, view);
		}
	}

	void nengine::render(const nviewport& view)
	{
		if( !m_renderer )
			return;

		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			(*iter)->render_scene_captures(view);
			(*iter)->render(view);
		}

		if (!m_level_list.empty())
		{
			m_renderer->present(view.handle_wnd);
		}
	}

	nrenderer_base* nengine::get_renderer()
	{
		return m_renderer.get();
	}

	nphysics_engine* nengine::get_phys_engine()
	{
		return m_physics_engine.get();
	}

	void nengine::close()
	{
		m_renderer->close(); // 注意:内部使用文件系统保存Shader Cache,所以必须保证文件系统此时是可用的

		m_level_list.clear();
		
		m_physics_engine->destroy();
		m_physics_engine.reset();

		resource_importer_manager::instance()->destroy();
		nresource_manager::instance()->close();

		m_render_res_mgr->destory();
		m_renderer.reset();	
		m_render_res_mgr.reset();
		m_file_sys.reset();
		m_plugin_array.clear();

		size_t res_leak = nresource_manager::instance()->get_num_resource_cached();
		if( res_leak != 0 )
		{
			nLog_Warning( _T("resource leak: %u"), res_leak );
		}
		nimage::close_image_sdk();
		
		// 关闭LOG
		//nlog::instance()->close();
	}

	nrender_resource_manager* nengine::get_render_res_mgr()
	{
		return m_render_res_mgr.get();
	}

	nfile_system* nengine::get_file_sys()
	{
		return m_file_sys.get();
	}

	nlevel::ptr nengine::create_level(const nstring& lv_name, const nstring& class_name)
	{
		// alloc
		nlevel::ptr new_lv(nconstruct<nlevel>(class_name, lv_name));
		nASSERT( new_lv );	

		// add to list
		m_level_list.push_back(new_lv);

		return new_lv;
	}

	void nengine::serialize_level_desc(const resource_location& folder, enum EFileMode fmode, nlevel_desc& ld)
	{
		resource_location file_loc = folder;
		file_loc.file_name += _T("/level_desc.xml");		
				
		narchive::ptr ar_ptr = narchive::open_xml_archive(get_file_sys(),
			fmode, file_loc.pkg_name, file_loc.file_name);		
		
		nstring dummy(_T("class_level_desc"));
		ar_ptr->object_begin(_T("level_desc"), dummy);
		
		ar_ptr->serial(_T("level_class"), ld.class_name);
		ar_ptr->serial(_T("object_name"), ld.object_name);
		ar_ptr->serial(_T("num_actors"), ld.num_actors);

		ar_ptr->object_end();

		ar_ptr->close();
	}

	void nengine::save_level(const nstring& lv_name, const resource_location& loc, nlevel_serialize_callback* callback)
	{
		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv_ptr = *iter;
			if(lv_ptr->get_name() == lv_name)
			{
				//-- 保存level描述信息
				nlevel_desc ld;
				ld.class_name = lv_ptr->reflection_get_class()->get_name();;
				ld.object_name = lv_ptr->get_name();
				ld.num_actors = lv_ptr->get_num_actors();
				serialize_level_desc(loc, EFileWrite, ld);

				if (callback)
					callback->set_level_desc(ld);				

				//-- 保存level对象
				lv_ptr->do_serialize(loc, EFileWrite, callback);
				break;
			}
		}
	}


	void nengine::export_level( const nstring& lv_name, const resource_location& loc )
	{
		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv_ptr = *iter;
			if(lv_ptr->get_name() == lv_name)
			{
				//-- 保存level对象
				lv_ptr->export_level(loc);
				break;
			}
		}
	}

	nlevel::ptr nengine::load_level(const resource_location& loc, nlevel_serialize_callback* callback)
	{
		nlevel_desc ld;
		serialize_level_desc(loc, EFileRead, ld);
		if (callback)
			callback->set_level_desc(ld);

		nlevel::ptr new_level = create_level(ld.object_name, ld.class_name);
		new_level->init();
		new_level->do_serialize(loc, EFileRead, callback);		
	
		return new_level;
	}

	void nengine::destroy_level(nlevel::ptr lv_ptr)
	{
		m_level_list.remove(lv_ptr);
		lv_ptr->_destroy();
	}

	void nengine::begin_play()
	{
		// for create physics data 
		for(st_level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			(*iter)->init_level_phys();
		}
	}

	void nengine::destroy_level(const nstring& lv_name)
	{
		nlevel::ptr lv_find;

		for(st_level_list::iterator iter = m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv = (*iter);
			if( lv->get_name() == lv_name )
			{
				lv_find = lv;
				break;
			}
		}

		if( lv_find )
			destroy_level( lv_find );
	}

	void nviewport::create_hit_hash()
	{
		nengine* eng = nengine::instance();
		const engine_config& cfg = eng->get_engine_config();

		hit_hash = 
			eng->get_render_res_mgr()->alloc_hit_proxy_hash();
		hit_hash->create(cfg.width, cfg.height);
		_enable_hit_hash = true;

		//-- 创建了资源之后，需要绑定device lost处理
		if( _device_handler_id==-1 )
		{
			nrenderer_base* rnd = nengine::instance()->get_renderer();
			_device_handler_id = rnd->register_device_handler(
				boost::bind(&nviewport::_on_device_lost, this, _1),
				boost::bind(&nviewport::_on_device_reset, this, _1) );
		}
	}

	nviewport::nviewport(void):x(0),y(0),width(800),height(600),min_z(0),max_z(1),
		handle_wnd(NULL),show_flags(0),render_mode(ERM_Lit),
		near_lod(200),far_lod(2000),lod_bias(0)
	{
		hit_hash = NULL;
		_enable_hit_hash = false;
		_device_handler_id = -1;
		enable_dynamic_shadow = true;
	}

	nviewport::~nviewport(void)
	{
		destroy();
	}

	void nviewport::destroy()
	{
		widgets_render.reset();
		if(hit_hash)
		{
			hit_hash->release();
			hit_hash = NULL;
		}
		
		if( _device_handler_id!=-1 )
		{
			nrenderer_base* rnd = nengine::instance()->get_renderer();
			rnd->unregister_device_handler(_device_handler_id);
			_device_handler_id = -1;
		}
	}

	void nviewport::_on_device_lost(int param)
	{
		UNREFERENCED_PARAMETER(param);
		if(hit_hash)
		{
			hit_hash->release();
			hit_hash = NULL;
		}
	}

	bool nviewport::_on_device_reset(int param)
	{
		UNREFERENCED_PARAMETER(param);
		if(_enable_hit_hash)
			create_hit_hash();
		return true;
	}

	void nengine::on_device_lost(int param)
	{
		for(st_level_list::iterator iter = m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv = (*iter);
			lv->_on_device_lost(param);
		}
	}

	bool nengine::on_device_reset(int param)
	{
		for(st_level_list::iterator iter = m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			nlevel::ptr lv = (*iter);
			bool ret = lv->_on_device_reset(param);
			if( !ret )
				return false;
		}
		return true;
	}

}//namespace nexus