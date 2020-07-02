#ifndef _NEXUS_RENDER_SKELETAL_MESH_H_
#define _NEXUS_RENDER_SKELETAL_MESH_H_

#include "nrender_mesh.h"

namespace nexus
{
	class ngpu_skin_mesh :
		public nrender_mesh
	{
	public:
		ngpu_skin_mesh(void)		{		}
		virtual ~ngpu_skin_mesh(void)	{		}
	};
}//namespace nexus

#endif // _NEXUS_RENDER_SKELETAL_MESH_H_