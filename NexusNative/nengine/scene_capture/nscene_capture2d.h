/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SCENE_CAPTURE_2D_H_
#define _NEXUS_SCENE_CAPTURE_2D_H_
#include "../framework/nscene_capture.h"

namespace nexus
{
	/**
	 * render a scene to a 2D texture target
	*/
	class nscene_capture2d
		: public nscene_capture
	{
	public:
		nscene_capture2d(void);
		virtual ~nscene_capture2d(void);
	};
}//namespace nexus
#endif //_NEXUS_SCENE_CAPTURE_2D_H_