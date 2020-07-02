#ifndef _NEXUS_ANIM_CONTROLLER_H_
#define _NEXUS_ANIM_CONTROLLER_H_
#include "ncore.h"
#include "anim_define.h"

namespace nexus
{
	class nAPI nanim_controller :
		public nobject
	{
	public:
		nanim_controller(void)			{		}
		virtual ~nanim_controller(void)	{		}

		//virtual bool ready() const = 0;

		nDECLARE_VIRTUAL_CLASS(nanim_controller)
	};
}//namespace nexus

#endif //_NEXUS_ANIM_CONTROLLER_H_