#ifndef _NEXUS_ACTOR_CONTROLLER_H_
#define _NEXUS_ACTOR_CONTROLLER_H_
#include "ncore.h"

namespace nexus
{
	class nactor;

	class nAPI nactor_controller
		: public nobject
	{
	public:
		typedef boost::shared_ptr<nactor_controller> ptr;

		nactor_controller(void) : m_actor(NULL)
		{}
		virtual ~nactor_controller(void)	{}

		virtual void _init(nactor* actor)
		{
			m_actor = actor;
		}
		virtual void update_actor(float delta_time) = 0;

	protected:
		nactor*	m_actor;

		nDECLARE_VIRTUAL_CLASS(nactor_controller)
	};
}//namespace nexus
#endif //_NEXUS_ACTOR_CONTROLLER_H_