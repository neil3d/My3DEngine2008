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

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);

		nactor::ptr pick_level_actor(const nviewport& view, int mouse_x, int mouse_y);		

		virtual void screen_shot(const nstring& file_name, size_t width, size_t height);
		virtual void viewport_shot(const nstring& file_name);
	private:
		nDECLARE_CLASS(ngame_engine);
	};
}//namespace nexus

#endif //_NEXUS_GAME_ENGINE_H_