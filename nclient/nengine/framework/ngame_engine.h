#ifndef _NEXUS_GAME_ENGINE_H_
#define _NEXUS_GAME_ENGINE_H_
#include "nengine.h"

namespace nexus
{
	class nAPI ngame_engine : public nengine
	{
	public:
		ngame_engine(void);
		virtual ~ngame_engine(void);

		virtual void frame_tick(float delta_time, const ncamera* cam, void* hWindow);
	};
}//namespace nexus

#endif //_NEXUS_GAME_ENGINE_H_