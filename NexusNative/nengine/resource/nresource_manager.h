/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRESOURCE_MANAGER_H_
#define _NRESOURCE_MANAGER_H_
#include <queue>
#include "ncore.h"
#include "nresource.h"
#include "nresource_cache.h"
#include "nresource_io.h"

#include "nresource_static_mesh.h"
#include "nresource_anim_mesh.h"
#include "nresource_skeletal_mesh.h"
#include "nresource_skeletal_anim_set.h"
#include "nresource_texture_atlas.h"
#include "../special_effect/nresource_special_effect.h"

#include "../framework/nevent.h"

#include "../material/nmtl_tech_shader_hlsl.h"
#include "../material/nmtl_static.h"

#include "../speed_tree/nspeed_base_tree.h"

namespace nexus
{
	const int NRESOURCE_EVENT_ID = 0x20000001;
	enum EResourceEventType
	{
		EResEvt_Ready,	// 加载完成
	};

	struct nresource_event : public nevent_base
	{
		nresource_event()
		{
			m_event_id = NRESOURCE_EVENT_ID;
		}

		EResourceEventType	type;
		nname				res_name;
	};

	class nresource_event_queue
	{
	public:
		nresource_event_queue(void)	{}
		~nresource_event_queue(void)	{}

		void push(const nresource_event& evt);		
		bool empty() const;
		void pop(nresource_event& out_evt);

	private:
		mutable boost::mutex		m_mutex;
		std::queue<nresource_event>	m_events;
	};

	/**
	 *	资源管理器
	*/
	class nAPI nresource_manager :
		public nobject
	{
	public:
		typedef shared_ptr<nresource_manager> ptr;

		nresource_manager(void);
		virtual ~nresource_manager(void);

		static nresource_manager* instance();

		void init(const nstring& cache_class, const nstring& io_class);
		void close();

		void register_event_target(nevent_target* obj);
		void unregister_event_target(nevent_target* obj);
		void post_event(const nresource_event& evt);
		void dispatch_events();

		nresource_static_mesh::ptr		new_static_mesh(const nstring& res_name);
		nresource_anim_mesh::ptr		new_anim_mesh(const nstring& res_name);
		nresource_skeletal_mesh::ptr	new_skeletal_mesh(const nstring& res_name);
		nresource_skeletal_anim_set::ptr	new_skeletal_anim(const nstring& res_name);
		nresource_special_effect::ptr	new_special_effect(const nstring& res_name);
		
		nresource_static_mesh::ptr	load_static_mesh(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_anim_mesh::ptr	load_anim_mesh(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_skeletal_anim_set::ptr load_skeletal_anim_set(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_skeletal_mesh::ptr load_skeletal_mesh(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_texture_2d::ptr		load_texture_2d(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_cube_map::ptr		load_cube_map(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_texture_atlas::ptr load_texture_atlas(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nmtl_tech_shader::ptr		load_mtl_shader(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_special_effect::ptr load_special_effect(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		void free_resource(nresource* res);
		nresource::ptr find_resource_by_name(const nstring& res_name);

		nmtl_base* get_default_material()				{	return &m_default_mtl;}
		nmtl_base* get_default_material_two_side()		{	return &m_default_mtl_two_sided;}
		size_t get_num_resource_cached() const				{	return m_cache->get_num_resource(); }
		nresource_texture_2d::ptr get_white_texture() const	{	return m_white_texture;}

		//-- speed tree
		//! 只支持阻塞模式的加载
		nspeed_base_tree::ptr load_speed_tree(const resource_location& loc);

	private:
		template<typename T>
		boost::intrusive_ptr<T> load_resource_T(const resource_location& loc, 
			enum EResourceIOMode mode, enum EResourceIOPriority pri);
		
		template<typename T>
		boost::intrusive_ptr<T> create_empty_resource_T(const nstring& res_name);

		void on_device_lost(int param);
		bool on_device_reset(int param);

	private:
		friend class nspt_resource_manager;//speed tree特殊资源管理器，公用cache，io对象

		scoped_ptr<nresource_cache>	m_cache;
		scoped_ptr<nresource_io>	m_io;
		
		nmtl_static					m_default_mtl;
		nmtl_static					m_default_mtl_two_sided;
		nresource_texture_2d::ptr	m_white_texture;

		std::set<nevent_target*>	m_event_targets;
		nresource_event_queue		m_events;
	};
}//namespace nexus
#endif //_NRESOURCE_MANAGER_H_