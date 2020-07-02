#ifndef _NEXUS_GAME_APP_H_
#define _NEXUS_GAME_APP_H_
#include "ncore.h"
#include "../lib_define.h"

namespace nexus
{
	class nClientAPI ngame_app
		: public nobject
	{
	public:
		ngame_app(void)	
		{	
			s_instance = this;
		}
		virtual ~ngame_app(void)	{	}

		static ngame_app* instance();

		virtual void create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style) = 0;
		virtual void close() = 0;
		virtual void main_loop() = 0;

		virtual void tick() = 0;

		nDECLARE_VIRTUAL_CLASS(ngame_app)

	private:
		static ngame_app* s_instance;
	};
}//namespace nexus

#endif //_NEXUS_GAME_APP_H_