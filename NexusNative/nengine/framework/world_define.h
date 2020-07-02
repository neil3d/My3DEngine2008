#ifndef _NEXUS_WORLD_DEFINE_H_
#define _NEXUS_WORLD_DEFINE_H_

namespace nexus
{
	const float WORLD_MAX		=	524288.0;	/* Maximum size of the world */
	const float HALF_WORLD_MAX	=	262144.0;	/* Half the maximum size of the world */
	const float HALF_WORLD_MAX1	=	262143.0;	/* Half the maximum size of the world - 1*/

	// result from a collision check.
	struct ncheck_result
	{
		vector3	location;
		vector3	normal;
		float	dist;

		ncheck_result(void)
		{
			dist = std::numeric_limits<float>::max();
		}
	};
}

#endif //_NEXUS_WORLD_DEFINE_H_