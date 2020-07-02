/**
 *	Nexus Engine
 *
 *	Author: Neil
 *	Date:	Aug, 2008
*/

#ifndef _NEXUS_ACTOR_H_
#define _NEXUS_ACTOR_H_

#include "ncore.h"
#include "nevent.h"
#include "world_define.h"
#include "../actor/nactor_component.h"
#include "nactor_controller.h"

namespace nexus
{
	class nlevel;
	class nrender_primitive_draw_interface;
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

		// called by level
		void _init(nlevel* level_ptr);
		void _remove_from_level();
		virtual void _destroy();

		virtual void update(float delta_time, const nviewport& view);
		virtual void on_event(nevent_base& evt);

		virtual void move(const vector3& pos, const vector3& rot, const vector3& scale);
		const matrix44& get_world_matrix() const	{	return m_space.local_to_world; }
		const object_space& get_space() const		{	return m_space;}	

		void set_visible(bool vis)		{	m_visible = vis;}	// 是否渲染
		void set_static(bool bStatic)	{	m_static = bStatic;}// 是否update
		const nname& get_name() const	{	return m_name;}
		const nstring& get_name_str() const	{	return m_name.name_str;}

		bool get_visible() const	{	return m_visible;}

		nactor_controller::ptr create_controller(const nstring& class_name);
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

		nlevel* get_owner()	{	return m_owner; }

		bool get_frozen() const	{	return m_frozen;}
		void set_frozen(bool val)	{	m_frozen = val;}
		bool get_serializable() const	{	return m_serializable;}
		void set_serializable(bool val)	{	m_serializable=val;}
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nlevel* level_ptr);

		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end)
		{
			(void)ret;
			(void)start;
			(void)end;
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
		void _set_name(const nstring& new_name)
		{
			m_name = nname(new_name);
		}
	protected:
		typedef std::list<nactor_component::ptr> st_component_list;

		nactor_controller::ptr		m_controller;
		st_component_list			m_component_list;		
		st_component_list::iterator	m_comp_iter;

		nlevel*			m_owner;
		nname			m_name;		
		bool			m_visible;
		bool			m_static;
		bool			m_serializable;	// 是否存盘
		bool			m_frozen;		// 是否在编辑器里被冻结(不可移动)
		object_space	m_space;		

		nDECLARE_NAMED_CLASS(nactor)
	};
}//namespace nexus

#endif //_NEXUS_ACTOR_H_