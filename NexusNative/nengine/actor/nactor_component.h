/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NACTOR_COMPONENT_H_
#define _NACTOR_COMPONENT_H_
#include "ncore.h"
#include "../framework/nevent.h"
#include "../framework/view_info.h"

namespace nexus
{
	class nactor;
	class nrender_primitive_draw_interface;
	/**
	 *	actor的一个功能组件
	 *	@remark 功能组件的基类, 渲染用的模型, 物理计算, 游戏事件皆派生于此
	*/
	class nAPI nactor_component :
		public nevent_target
	{
	public:
		typedef shared_ptr<nactor_component> ptr;
		
		nactor_component(const nstring& name_str);
		virtual ~nactor_component(void);

		// called by actor
		virtual void _init(nactor* owner);
		virtual void _update_transform(const object_space& parent_space)	{	(void)parent_space; }
		virtual void _remove_from_level() {}
		virtual void _destroy()	{}

		virtual void update(float delta_time, const nviewport& view)	
		{
			(void)delta_time;
			(void)view;
		}
		
		virtual void render(const nviewport& view)	{	(void)view;	}
		nactor* get_owner()				{	return m_owner;}
		const nname& get_name() const	{	return m_name;}

		virtual void on_event(nevent_base& evt)	{ (void)evt;}

		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);

		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color)
		{
			(void)PDI;
			(void)color;
		}
	protected:
		nactor*	m_owner;
		nname	m_name;

		nDECLARE_VIRTUAL_CLASS(nactor_component)
	};
}//namespace nexus
#endif //_NACTOR_COMPONENT_H_