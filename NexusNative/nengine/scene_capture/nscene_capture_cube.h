/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SCENE_CAPTURE_CUBE_H_
#define _NEXUS_SCENE_CAPTURE_CUBE_H_
#include "../framework/nscene_capture.h"

namespace nexus
{
	/**
	 *	render a scene to a cube texture target using 6 passes
	*/
	class nscene_capture_cube
		: public nscene_capture
	{
	public:
		nscene_capture_cube(void);
		virtual ~nscene_capture_cube(void);
	};
}//namespace nexus
#endif //_NEXUS_SCENE_CAPTURE_CUBE_H_