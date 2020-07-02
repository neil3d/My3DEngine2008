#ifndef _NEXUS_EVENT_H_
#define _NEXUS_EVENT_H_
#include "ncore.h"

namespace nexus
{
	struct nevent_base
	{
		int	m_event_id;
	};

	class nAPI nevent_target
		: public nobject
	{
	public:
		nevent_target(void)	{}
		virtual ~nevent_target(void)	{}

		virtual void on_event(nevent_base& evt) = 0;

		nDECLARE_VIRTUAL_CLASS(nevent_target)
	};

}

#endif // end of _NEXUS_EVENT_H_