#ifndef _NEXUS_SCENE_EVNET_H_
#define _NEXUS_SCENE_EVNET_H_
#include "../framework/nevent.h"
#include "../actor/nprimitive_component.h"
#include "../actor/nlight_component.h"

namespace nexus
{
	enum ESceneAction
	{
		ESA_AddPrimitive,
		ESA_RemovePrimitive,
		ESA_MovePrimitive,

		ESA_AddLight,
		ESA_RemoveLight,
		ESA_MoveLight,
	};

	const int NSCENE_EVENT_ID = 0x10000001;
	struct nscene_event
		: public nevent_base
	{
		enum ESceneAction		m_action;
		
		nprimitive_component*	m_prim;
		nlight_component*		m_light;

		nscene_event(enum ESceneAction act, nprimitive_component* prim)
			:m_action(act), m_prim(prim)
		{
			m_event_id = NSCENE_EVENT_ID;
			m_light = NULL;
		}

		nscene_event(enum ESceneAction act, nlight_component* lgt)
			:m_action(act), m_light(lgt)
		{
			m_event_id = NSCENE_EVENT_ID;
			m_prim = NULL;
		}
	};
};

#endif //_NEXUS_SCENE_EVNET_H_