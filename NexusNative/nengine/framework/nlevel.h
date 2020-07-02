/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_LEVEL_H_
#define _NEXUS_LEVEL_H_
#include "ncore.h"
#include "math/ncheck_result.h"
#include "nevent.h"
#include "../scene/nscene_manager.h"
#include "nactor.h"
#include "view_info.h"
#include "nscene_capture.h"
#include "../physics/nphys_scene.h"
#include "../../ncore/game_map/navigation_map.h"
#include "ngameframework.h"
#include "../renderer/nrenderer_base.h"
#include "../resource/nresource.h"

namespace nexus
{
	class nhit_proxy_hash;
	class nlevel_info_resource;

	struct nlevel_desc
	{
		nstring class_name;
		nstring object_name;
		size_t	num_actors;
	};

	struct nlevel_layer_desc
	{
		bool	is_visible;		//层是否可见
		bool	is_locked;		//层是否被锁定
	};

	//	加载level时候的callback接口，通知上层相关的事件
	//	可以用来做进度条，或者是actor的一些定制处理等等
	struct nlevel_serialize_callback
	{
		virtual void set_level_desc(const nlevel_desc& ld) = 0;
		virtual void begin_actors() = 0;
		virtual void actor_loaded(nactor::ptr obj) = 0;
		virtual void actor_saved(nactor::ptr obj) = 0;
		virtual void end_actors() = 0;
	};

	/**
	 *	关卡的基类
	 *	@remark 组合actors和level geom对象，前者主要代表可交互的，后者主要用作静态装饰（没有游戏逻辑，但可以有动画）
	 *	关于存盘/读盘：基本对象描述存入一个xml文件中；如果有的对象需要存取大块的二进制数据，可以单独存入一个bin文件中。
	*/
	class nAPI nlevel :
		public nevent_target
	{
	public:
		typedef shared_ptr<nlevel> ptr;
		typedef shared_ptr<nlevel_layer_desc> layer_ptr;

		explicit nlevel(const nstring& name_str);
		virtual ~nlevel(void);

		virtual void init();
		virtual void _destroy();

		// physics interface
		virtual void init_level_phys( const vector3& gravity = vector3(0.f,-9.8f,0.f) );

		nphys_scene* get_phys_scene()
		{
			return m_phys_scene.get();
		}
		
		virtual nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name=_T("nactor"));
		virtual bool destroy_actor(const nname& actor_name);
		nactor::ptr find_actor(unsigned int name_crc);
		nactor::ptr find_actor(const nstring& name_str);

		//复制指定的actor,新actor出现在原始actor的位置并且和原始actor完全一致，返回新actor的指针
		nactor::ptr clone_actor(nactor::ptr actor);

		bool rename_actor(const nstring& old_name, const nstring& new_name);

		//! 把一个已经创建好的NActor添加到关卡中
		void attach_actor(nactor::ptr actor);
		//! 把一个已经创建好的NActor从关卡中移除，但是并不删除该NActor
		bool deattach_actor(nactor::ptr actor);

		bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type);

		template<typename NClass>
		shared_ptr<NClass> create_actor_t(const nstring& obj_name)
		{
			return dynamic_pointer_cast<NClass>( 
				create_actor(obj_name, 
					NClass::reflection_get_static_class()->get_name())
				);
		}

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);
		virtual void on_event(nevent_base& evt);
		void render_scene_captures(const nviewport& view);

		void destroy_all_actors();

		nscene_manager::ptr get_scene_manager() {return m_scene_ptr;}
		const nstring& get_name() const	{	return m_name; }

		void add_scene_capture(nscene_capture::ptr cap);
		void remove_scene_capture(nscene_capture::ptr cap);

		void do_serialize(const struct resource_location& folder, enum EFileMode fmode, nlevel_serialize_callback* callback);
		virtual void serialize(narchive& ar);

		size_t get_num_actors() const	{	return m_all_actor.size(); }
		nactor::ptr first_actor()
		{
			m_actor_iter = m_all_actor.begin();
			return m_actor_iter->second;
		}
		nactor::ptr next_actor()
		{
			nactor::ptr ret;
			
			++m_actor_iter;
			if( m_actor_iter != m_all_actor.end() )
				ret = m_actor_iter->second;
			return ret;
		}

		render_setting*	get_render_setting()
		{
			return &m_render_setting;
		}

		//-- layer API
		unsigned int  get_layer_count();
		nstring get_layer_name(unsigned int index);
		bool is_layer_exist(const nstring& layer_name);
		void new_layer(const nstring& layer_name);
		void remove_layer(const nstring& layer_name);
		//重命名层
		void rename_layer(const nstring& old_layer_name,const nstring& new_layer_name);
		//设置当前被激活的层
		void set_active_layer(const nstring& layer_name);
		//获取当前被激活的层的名字
		nstring get_active_layer();
		//锁定或者解锁指定名字的层
		void lock_layer(const nstring& layer_name,bool lock);
		//指定名字的层是否被锁定
		bool is_layer_locked(const nstring& layer_name);
		//显示或者隐藏指定的层
		void show_layer(const nstring& layer_name,bool show);
		//是否显示指定层
		bool is_layer_visible(const nstring& layer_name);
		//将指定的actor移动到当前激活层
		void move_actor_to_active_layer(const nstring& actor_name);

		// 导航图相关接口
		navigation_map::ptr get_navigation_map() const { return m_nav_map; }
		// ref navigation_map::line_of_sight
		bool line_of_sight(ncheck_result& ret, const vector3& start, const vector3& end, bool walkable_check);

		bool get_world_walkable(float wx, float wz) const { return m_nav_map->get_world_walkable(wx,wz); }
		bool get_tile_walkable(int tx, int tz) const { return m_nav_map->get_tile_walkable(tx,tz); }
		float get_world_height(float wx, float wz) const { return m_nav_map->get_world_height(wx,wz); }
		float get_tile_height(int tx, int tz) const { return m_nav_map->get_tile_height(tx,tz); }

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

		/**
		* 导出level，导出和序列化不同的是，导出是为了提供服务器运行需要的资源
		* param pkg_name: 导出包名
		* param file_path: 导出文件路径，实际存盘文件名为 file_path + _T("/level_info_resource.xml")
		* return 总计导出了多少个actor
		*/
		virtual int export_level(const nstring& pkg_name, const nstring& file_path);
		virtual int export_level(const resource_location& res_loc);

	protected:
		//-- 遍历actor导出，有导出内容则返回true
		virtual bool export_actor(gameframework::nlevel_info_resource* res, nactor::ptr actor);

		// 当序列化函数完成load数据时调用，用来进行特殊的初始化操作
		// 因为序列化并不能完全恢复对象状态，有些运行时创建的对象需要在这里特殊处理
		virtual void serialize_loaded();
		virtual void serialize_layer_name(narchive& ar, const nstring& layer_name,nlevel_serialize_callback* callback);

		//-- 存取导航图信息
		virtual void serialize_navigation_map(nfile_system* fs, const resource_location& level_folder, enum EFileMode fmode);

		// simulate level physics
		virtual void simulate_level_phys(float delta_time);
		virtual void get_level_phys_result();
		virtual void release_level_phys();
	protected:
		typedef std::map<unsigned int, nactor::ptr>		nactor_map;
		typedef std::set<nstring> st_string_set;
		typedef std::list<nscene_capture::ptr>		nscene_capture_list;
		typedef std::map<nstring,nlevel_layer_desc>	nlayer_map;

		/**
		 * scene captures对象列表
		 * @remarks 现在假设一个level中的capture的数量不会很多，所以使用一个list + per object frustum cull
		*/
		nscene_capture_list		m_all_captures;
		nactor_map				m_all_actor;	// todo:目前的scene capture管理方式，需要确保actor先析构
		nactor_map::iterator	m_actor_iter;

		st_string_set		m_layer_set;
		nlayer_map			m_layer_desc;
		nstring				m_active_layer;

		nstring				m_scene_class_name;
		nscene_manager::ptr	m_scene_ptr;
		
		// physics data
		nphys_scene::ptr	m_phys_scene;

		render_setting		m_render_setting;
	private:
		nstring	m_name;

		//! 导航图信息
		navigation_map::ptr		m_nav_map;

		nDECLARE_NAMED_CLASS(nlevel)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_H_