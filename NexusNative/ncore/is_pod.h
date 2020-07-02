#ifndef _NEXUS_IS_POD_H_
#define _NEXUS_IS_POD_H_

namespace nexus
{

	template<typename T>
	struct IS_POD
	{
		static const bool value = false;		
	};
}//namespace nexus

#endif