#ifndef _NEXUS_VIEW_INFO_H_
#define _NEXUS_VIEW_INFO_H_
#include "ncore.h"

namespace nexus
{
	struct view_info
	{
		const ncamera*	cam;
		nfrustum		frustum;
		void*			handle_wnd;
		unsigned long	show_flags;
	};
}//namespace nexus

#endif //_NEXUS_VIEW_INFO_H_