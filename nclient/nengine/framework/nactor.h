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
#include "../actor/nactor_component.h"

namespace nexus
{
	class nlevel;
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

		virtual void update_component(float delta_time, const view_info& view);
		virtual void on_event(nevent_base& evt);

		void move(const matrix44& mat);
		const matrix44& get_world_matrix()			{	return m_mat_world; }

		void set_visible(bool vis)		{	m_visible = vis;}	// 是否渲染
		void set_static(bool bStatic)	{	m_static = bStatic;}// 是否update
		const nname& get_name() const	{	return m_name;}

		bool get_visible() const	{	return m_visible;}

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
			for(component_list::iterator iter = m_component_list.begin();
				iter != m_component_list.end(); ++iter)
			{
				if( (*iter)->get_name() == find_name)
					return *iter;
			}
			return nactor_component::ptr();
		}

	private:
		typedef std::list<nactor_component::ptr> component_list;

		nlevel*			m_owner;
		nname			m_name;
		component_list	m_component_list;		
		bool			m_visible;
		bool			m_static;
		matrix44		m_mat_world;

		nDECLARE_NAMED_CLASS(nactor)
	};
}//namespace nexus

#endif //_NEXUS_ACTOR_H_