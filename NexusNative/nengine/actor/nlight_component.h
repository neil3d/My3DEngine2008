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

		virtual void render(const nviewport& view);		
		virtual void _update_transform(const object_space& parent_space);
		virtual void _remove_from_level();
		virtual void _destroy();
		//--
		void create_point_light();
		npoint_light_proxy* get_point_light();

		void create_directional_light();
		ndirectional_light_proxy* get_directional_light();

		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	private:
		shared_ptr<nrender_light_proxy>	m_light_proxy;

		nDECLARE_NAMED_CLASS(nlight_component)
	};
}//namespace nexus

#endif //_NEXUS_LIGHT_COMPONENT_H_