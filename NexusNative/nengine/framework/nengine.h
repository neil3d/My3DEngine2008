/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_ENGINE_H_
#define _NEXUS_ENGINE_H_
#include "ncore.h"
#include "nlevel.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "view_info.h"
#include "../physics/nphysics_engine.h"

namespace nexus
{
	class nrender_resource_manager;

	struct engine_config : public render_config
	{
		nstring	renderer_class;		// 渲染系统class name		
		nstring	file_sys_root;		// 文件系统的根目录
		nstring file_system_class;	// 文件系统class name	
		nstring engine_data_pkg;	// 引擎数据文件所在的package name
		nstring	resource_cache_class;	// 资源缓存策略类的class name
		nstring resource_io_class;		// 资源IO策略类的class name

		// physics engine config
		phys_config phys_cfg;
	};

	/**
	 *	Nexus Engine Facade类
	 *	@remark 组合引擎的各种组件, 提供访问接口
	*/
	class nAPI nengine :
		public nobject
	{
	public:
		nengine(void);
		virtual ~nengine(void);

		static nengine* instance();

		virtual void init_core();
		virtual void init_modules(const engine_config& cfg);
		virtual void close();		

		nlevel::ptr create_level(const nstring& lv_name, const nstring& class_name);
		void save_level(const nstring& lv_name, const resource_location& loc, nlevel_serialize_callback* callback=NULL);
		nlevel::ptr load_level(const resource_location& loc, nlevel_serialize_callback* callback=NULL);
		// 导出Level
		void export_level(const nstring& lv_name, const resource_location& loc);

		void destroy_level(nlevel::ptr lv_ptr);
		void destroy_level(const nstring& lv_name);
		
		// begin play just like Unreal 
		virtual void begin_play();

		nrenderer_base* get_renderer();		
		nrender_resource_manager* get_render_res_mgr();
		nfile_system* get_file_sys();
		const nstring& get_engine_package_name() const	{	return m_config.engine_data_pkg;}
		nphysics_engine* get_phys_engine();

		const engine_config& get_engine_config() const	{	return m_config;}
	protected:
		virtual void update(float delta_time,const nviewport& view);
		virtual void render(const nviewport& view);

		size_t load_plugins();

		void serialize_level_desc(const resource_location& folder, enum EFileMode fmode, nlevel_desc& ld);	

		void on_device_lost(int param);
		bool on_device_reset(int param);

	protected:
		static nengine* s_engine_ptr;		

		typedef std::list<nlevel::ptr> st_level_list;

		engine_config	m_config;
		
		st_level_list					m_level_list;
		nrenderer_base::ptr				m_renderer;
		nrender_resource_manager::ptr	m_render_res_mgr;
		vector<nshared_lib::ptr>		m_plugin_array;
		nfile_system::ptr				m_file_sys;

		// physics engine
		nphysics_engine::ptr			m_physics_engine;

		friend class nengine_unit_test;
		nDECLARE_CLASS(nengine);
	};
}//namespace nexus
#endif _NEXUS_ENGINE_H_