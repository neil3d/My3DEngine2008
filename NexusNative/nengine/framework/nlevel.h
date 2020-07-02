/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_LEVEL_H_
#define _NEXUS_LEVEL_H_
#include "ncore.h"
#include "nevent.h"
#include "../scene/nscene_manager.h"
#include "nactor.h"
#include "view_info.h"
#include "nscene_capture.h"

namespace nexus
{
	class nhit_proxy_hash;

	//	加载level时候的callback接口，通知上层相关的事件
	//	可以用来做进度条，或者是actor的一些定制处理等等
	struct nlevel_serialize_callback
	{
		virtual void begin_actors(size_t total_actors) = 0;
		virtual void actor_loaded(size_t index, nactor::ptr obj) = 0;
		virtual void actor_saved(size_t index, nactor::ptr obj) = 0;
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

		nlevel(const nstring& name_str);
		virtual ~nlevel(void);

		virtual void init(const nstring& scene_class_name);
		virtual void _destroy();

		virtual nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name=_T("nactor"));
		virtual void destroy_actor(const nname& actor_name);
		nactor::ptr find_actor(unsigned int name_crc);
		nactor::ptr find_actor(const nstring& name_str);

		void rename_actor(const nstring& old_name, const nstring& new_name);

		bool line_check(ncheck_result& ret, const vector3& start, const vector3& end);

		template<typename NClass>
		shared_ptr<NClass> create_actor_t(const nstring& obj_name)
		{
			return dynamic_pointer_cast<NClass>( 
				create_actor(obj_name, 
					NClass::reflection_get_static_class()->get_name())
				);
		}

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view, nhit_proxy_hash* hph);
		virtual void on_event(nevent_base& evt);
		void render_scene_captures(const nviewport& view);

		void destroy_all_actors();

		nscene_manager::ptr get_scene_manager() {return m_scene_ptr;}
		const nstring& get_name() const	{	return m_name; }

		void add_scene_capture(nscene_capture::ptr cap);
		void remove_scene_capture(nscene_capture::ptr cap);
		
		static void set_serialize_callback(nlevel_serialize_callback* callback)
		{
			s_serialize_callback = callback;
		}

		virtual void serialize(narchive& ar);

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

	protected:
		// 当序列化函数完成load数据时调用，用来进行特殊的初始化操作
		// 因为序列化并不能完全恢复对象状态，有些运行时创建的对象需要在这里特殊处理
		virtual void serialize_loaded();
	protected:
		typedef std::map<unsigned int, nactor::ptr>		nactor_map;
		typedef std::list<nscene_capture::ptr>		nscene_capture_list;
		
		/**
		 * scene captures对象列表
		 * @remarks 现在假设一个level中的capture的数量不会很多，所以使用一个list + per object frustum cull
		*/
		nscene_capture_list		m_all_captures;
		nactor_map				m_all_actor;	// todo:目前的scene capture管理方式，需要确保actor先析构
		nactor_map::iterator	m_actor_iter;

		nstring				m_scene_class_name;
		nscene_manager::ptr	m_scene_ptr;
		
		static nlevel_serialize_callback* s_serialize_callback;

	private:
		nstring	m_name;

		nDECLARE_NAMED_CLASS(nlevel)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_H_