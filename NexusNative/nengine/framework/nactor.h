/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_ACTOR_H_
#define _NEXUS_ACTOR_H_

#include "ncore.h"
#include "math/ncheck_result.h"
#include "nevent.h"
#include "world_define.h"
#include "../actor/nactor_component.h"
#include "../physics/nphys_define.h"
#include "../../NexusGame/ngameframework/ngameframework.h"

namespace nexus
{
	const nstring ACTOR_DEFAULT_LAYER = _T("default");

	class nlevel;
	class nrender_primitive_draw_interface;
	class gameframework::nlevel_info_resource;
	/**
	*	level中的所有可交互对象的基类
	*	@remark level中绝大多数对象的基类, 渲染, 碰撞, 游戏事件等功能都可以通过component组合到其中;
	*/
	class nAPI nactor :
		public nevent_target
	{
	public:
		typedef shared_ptr<nactor> ptr;

		nactor(const nstring& name_str);
		virtual ~nactor(void);		
		
		//产生一个深度复制的副本,但是m_controller不被复制
		virtual nactor::ptr clone();

		// called by level
		virtual void _init(nlevel* level_ptr);
		virtual void _remove_from_level();
		virtual void _destroy();
		bool is_valid() const	{	return m_owner!=NULL; }

		virtual void update(float delta_time, const nviewport& view);

		// physics interfaces
		// init actor physics,应当在begin play时才调用该接口 
		virtual void init_phys();
		virtual void simulate_phys(float delta_time);
		virtual void release_phys();
		virtual void sync_actor_to_physics();

		virtual void on_event(nevent_base& evt);
		
		virtual void look_at(const vector3 & at, const vector3 & up = vector3(0,1,0));
		virtual void move(const vector3& pos, const vector3& rot, const vector3& scale);
		virtual void move(const vector3& pos);
		virtual void update_cmp_transform();
		const matrix44& get_world_matrix() const	{	return m_space.local_to_world; }
		const object_space& get_space() const		{	return m_space;}	
		enum EDepthGroup get_depth_group() const	{	return m_depth_group; }
		void set_depth_group(enum EDepthGroup dg)	{	m_depth_group = dg; }

		virtual void set_location( const vector3& location );
		virtual void set_rotation( const vector3& rot );

		void set_visible(bool vis)		{	m_visible = vis;}	// 是否渲染
		void set_static(bool bStatic)	{	m_static = bStatic;}// 是否update
		bool get_static() const	{	return m_static; }	
		const nname& get_name() const	{	return m_name;}
		const nstring& get_name_str() const	{	return m_name.name_str;}

		bool get_visible() const	{	return m_visible; }

		nactor_component::ptr create_component(const nstring& component_name, const nstring& class_name);		

		template<typename NClass>
		shared_ptr<NClass> create_component_t(const nstring& component_name)
		{
			return dynamic_pointer_cast<NClass>(
				create_component(component_name,
				NClass::reflection_get_static_class()->get_name())
				);
		}

		nactor_component::ptr get_component(const nstring& name_str)
		{
			nname find_name(name_str);
			for(st_component_list::iterator iter = m_component_list.begin();
				iter != m_component_list.end(); ++iter)
			{
				if( (*iter)->get_name() == find_name)
					return *iter;
			}
			return nactor_component::ptr();
		}

		// 移除nactor_component
		bool remove_component(const nname& name_str);

		bool remove_component(nactor_component::ptr component)
		{
			return remove_component(component->get_name());
		}

		bool remove_all_component();

		//判断是否和指定模型拥有相同的部件
		bool has_same_type_component(nactor* actor);

		nlevel* get_owner()	{	return m_owner; }

		bool get_frozen() const	{	return m_frozen;}
		void set_frozen(bool val)	{	m_frozen = val;}
		bool get_serializable() const	{	return m_serializable;}
		void set_serializable(bool val)	{	m_serializable=val;}
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nlevel* level_ptr);

		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type)
		{
			(void)ret;
			(void)start;
			(void)end;
			(void)check_type;
			return false;
		}

		//-- Editor functions
		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color);

		size_t get_num_components() const	{	return m_component_list.size();}
		nactor_component::ptr first_component()
		{
			m_comp_iter = m_component_list.begin();
			return *m_comp_iter;
		}

		nactor_component::ptr next_component()
		{
			nactor_component::ptr ret;
			++m_comp_iter;
			if( m_comp_iter != m_component_list.end() )
				ret = *m_comp_iter;
			return ret;
		}

		// don't call this, call nlevel::rename_actor() instead.
		virtual void _set_name(const nstring& new_name)
		{
			m_name = nname(new_name);
		}

		void set_layer(const nstring& layer_name)
		{
			m_layer_name =layer_name;
		}
		const nstring& get_layer_name() const
		{
			return m_layer_name;
		}

		EPhysicsState get_physics_state() const { return m_physics_state; }
		void set_physics_state( EPhysicsState state ) { m_physics_state = state; }

		nactor_component::ptr get_collision_cmp() const { return m_collision_cmp; }
		void set_collision_cmp( nactor_component::ptr collision_cmp ){ m_collision_cmp = collision_cmp; };

		bool is_kinematic() const { return m_kinematic; }
		void set_kinematic(bool bkinematic)	{ m_kinematic = bkinematic; }

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

		//-----------------------------------------------------------------------------------
		//如果所拥有的模型里面有mesh_component及其派生类，则将它们设置为实例渲染模式或者非实例渲染模式
		virtual void set_enable_instance(bool is_enable );

		//-- 导出资源接口,缺省返回false(什么都不导出)
		virtual bool export_info_resource(gameframework::nlevel_info_resource* res);
	protected:
		void serialize_basic_property(narchive& ar);

	protected:
		typedef std::list<nactor_component::ptr> st_component_list;

		st_component_list			m_component_list;		
		st_component_list::iterator	m_comp_iter;

		nlevel*			m_owner;
		nname			m_name;		
		nstring			m_layer_name;	// 所属的关卡layer
		bool			m_visible;
		bool			m_static;
		bool			m_serializable;	// 是否存盘
		bool			m_frozen;		// 是否在编辑器里被冻结(不可移动)
		object_space	m_space;
		enum EDepthGroup m_depth_group;

		// physics data
		bool						m_kinematic; // 是否是可运动的，一位着是动态的Actor并且由逻辑层控制Actor运动
		EPhysicsState				m_physics_state;
		nactor_component::ptr		m_collision_cmp;

		nDECLARE_NAMED_CLASS(nactor)
	};
}//namespace nexus

#endif //_NEXUS_ACTOR_H_