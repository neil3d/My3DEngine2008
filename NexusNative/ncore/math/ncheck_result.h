#ifndef _NEXUS_CHECK_RESULT_H_
#define _NEXUS_CHECK_RESULT_H_
#include <limits>

namespace nexus
{
	// result from a collision check.
	struct ncheck_result
	{
		vector3	location;
		vector3	normal;
		float	dist;

		ncheck_result(void)
		{
			dist = (std::numeric_limits<float>::max)();
		}
	};
}

#endif //_NEXUS_CHECK_RESULT_H_