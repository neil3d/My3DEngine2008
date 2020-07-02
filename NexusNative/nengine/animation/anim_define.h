#ifndef _NEXUS_ANIM_DEFINE_H_
#define _NEXUS_ANIM_DEFINE_H_
#include "../mesh/anim_mesh.h"

namespace nexus
{
	enum EAnimLoop
	{
		EAnim_Loop,
		EAnim_PlayOnce,
	};

	struct anim_mesh_morph
	{
		nmesh_vertex_data::ptr	src_frame_vert;
		nmesh_vertex_data::ptr	dst_frame_vert;
		float	alpha;

		void clear()
		{
			src_frame_vert.reset();
			src_frame_vert.reset();
		}
	};
}//namespace nexus

#endif //_NEXUS_ANIM_DEFINE_H_