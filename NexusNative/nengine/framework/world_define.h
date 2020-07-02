#ifndef _NEXUS_WORLD_DEFINE_H_
#define _NEXUS_WORLD_DEFINE_H_

namespace nexus
{
	/// line_check参数
	enum ELineCheckType
	{
		ELCT_Static		= (1<<1),		 //!< 只检测静态的物体
		ELCT_Dynamic	= (1<<2),		 //!< 只检测动态的物体

		ELCT_Terrain	= (1<<3),		 //!< 只检测地形
		ELCT_Navigation	= (1<<4),		 //!< 只检测导航图

		// combinations.
		ELCT_World		= ELCT_Static | ELCT_Terrain,

		ELCT_All = 0x7FFFFFFF
	};
}

#endif //_NEXUS_WORLD_DEFINE_H_