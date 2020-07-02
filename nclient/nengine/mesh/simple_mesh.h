#ifndef _NEXUS_SIMPLE_MESH_H_
#define _NEXUS_SIMPLE_MESH_H_
#include "ncore.h"

namespace nexus
{
	/*	一个static mesh的简化模型
	 *	@remarks 可用来做碰撞检测、occlusion cull，shadow等
	*/
	struct nsimple_mesh
	{
		typedef boost::shared_ptr<nsimple_mesh> ptr;

		std::vector<vector3>		vert_buffer;
		std::vector<unsigned short>	index_buffer;
	};
}//namespace nexus

#endif //_NEXUS_SIMPLE_MESH_H_