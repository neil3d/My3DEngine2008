/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NSUBSYSTEM_H_
#define _NSUBSYSTEM_H_
#include "ncore.h"

namespace nexus
{
	/**
	 *	子系统基类
	*/
	class nAPI nsubsystem :
		public nobject
	{
	public:
		nsubsystem(void)	{}
		virtual ~nsubsystem(void)	{}

		nDECLARE_VIRTUAL_CLASS(nsubsystem)
	};
}//namespace nexus

#endif //_NSUBSYSTEM_H_