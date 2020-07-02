#include "StdAfx.h"
#include "nscene_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nscene_manager, nobject)

	nscene_manager::nscene_manager(void)
	{
	}

	nscene_manager::~nscene_manager(void)
	{
	}

	void nscene_manager::on_event(nevent_base& evt)
	{
		if( evt.m_event_id != NSCENE_EVENT_ID )
			return;

		nscene_event& scn_evt = static_cast<nscene_event&>(evt);

		switch(scn_evt.m_action)
		{
		case ESA_AddPrimitive:
			add_primitive(scn_evt.m_prim);
			break;
		case ESA_RemovePrimitive:
			remove_primitive(scn_evt.m_prim);
			break;
		case ESA_MovePrimitive:
			move_primitive(scn_evt.m_prim);
			break;
		case ESA_AddLight:
			add_light(scn_evt.m_light);
			break;
		case ESA_RemoveLight:
			remove_light(scn_evt.m_light);
			break;
		case ESA_MoveLight:
			move_light(scn_evt.m_light);
			break;
		default:
			nASSERT( 0 && "unknown scene event action");
			break;
		}
	}
}//namespace nexus