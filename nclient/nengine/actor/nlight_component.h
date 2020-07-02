/**
*	Nexus Engine - 灯光组件
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_LIGHT_COMPONENT_H_
#define _NEXUS_LIGHT_COMPONENT_H_
#include "nactor_component.h"
#include "../renderer/nrender_light.h"

namespace nexus
{
	/**
	 *	灯光
	 *	@remark Actor space
	*/
	class nAPI nlight_component :
		public nactor_component
	{
	public:
		nlight_component(const nstring& name_str);
		virtual ~nlight_component(void);

		virtual void _init(nactor* owner);

		virtual void render(const view_info& view);
		virtual void update(float delta_time, const view_info& view);
		virtual void _on_actor_move();
		virtual void _remove_from_level();
		//--
		void create_point_light();
		npoint_light_proxy* get_point_light();

		void create_directional_light();
		ndirectional_light_proxy* get_directional_light();

	private:
		shared_ptr<nrender_light_proxy>	m_light_proxy;

		nDECLARE_NAMED_CLASS(nlight_component)
	};
}//namespace nexus

#endif //_NEXUS_LIGHT_COMPONENT_H_