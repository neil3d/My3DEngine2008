#ifndef _NEXUS_SPEED_TREE_COMPONENT_H_
#define _NEXUS_SPEED_TREE_COMPONENT_H_
#include "../actor/nprimitive_component.h"
#include "../resource/nresource.h"
#include "nspt_instance.h"

namespace nexus
{
	/**
	 *	包装一个speed tree instance在Actor中的表现
	*/
	class nAPI nspeed_tree_component
		: public nprimitive_component
	{
	public:
		typedef boost::shared_ptr<nspeed_tree_component> ptr;

		nspeed_tree_component(const nstring& name_str);
		virtual ~nspeed_tree_component(void);

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);
		virtual void _destroy();

		void create(const resource_location& spt_file);
		virtual void _update_transform(const object_space& parent_space);

		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	private:
		resource_location	m_spt_file_loc;
		nspt_instance		m_spt;

		nDECLARE_NAMED_CLASS(nspeed_tree_component)
	};
}//namespace nexus
#endif //_NEXUS_SPEED_TREE_COMPONENT_H_