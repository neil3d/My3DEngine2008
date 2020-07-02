#include "StdAfx.h"
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

		nlog::instance()->init(_T("nexus.log"));
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

		//-- render resource manager
		nstring render_res_mgr_class_name = m_renderer->get_resource_manager_class_name();
		m_render_res_mgr.reset(create_plugin_class_object<nrender_resource_manager>(render_res_mgr_class_name));

		nresource_manager::instance()->init();
		//--
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

	void nengine::update(float delta_time)
	{
		for(level_list::iterator iter = m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			(*iter)->update(delta_time, m_cur_view);
		}
	}

	void nengine::render(const view_info& view)
	{
		for(level_list::iterator iter= m_level_list.begin();
			iter != m_level_list.end();
			++iter)
		{
			(*iter)->render(view);
		}
	}

	nrenderer_base* nengine::get_renderer()
	{
		return m_renderer.get();
	}

	void nengine::close()
	{
		m_level_list.clear();

		resource_importer_manager::instance()->destroy();
		m_renderer.reset();		
		m_render_res_mgr.reset();
		m_file_sys.reset();
		m_plugin_array.clear();
		
		nimage::close_image_sdk();
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

	void nengine::render_attach_element(nrender_element::ptr element_ptr)
	{
		m_renderer->attach_element(element_ptr);
	}

	void nengine::render_detach_element(nrender_element::ptr element_ptr)
	{
		m_renderer->detach_element(element_ptr);
	}

	void nengine::set_render_mode(enum ERenderMode rm)
	{
		m_renderer->set_render_mode(rm);
	}

	enum ERenderMode nengine::get_render_mode() const
	{
		return m_renderer->get_render_mode();
	}
}//namespace nexus