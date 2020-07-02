#ifndef _NEXUS_RENDER_ANIM_MESH_H_
#define _NEXUS_RENDER_ANIM_MESH_H_
#include "nrender_mesh.h"
#include "../mesh/anim_mesh.h"
#include "../animation/anim_define.h"

namespace nexus
{
	// indexed
	class nAPI nrender_anim_mesh 
		: public nrender_mesh
	{
	public:
		nrender_anim_mesh(void)		{		}
		~nrender_anim_mesh(void)	{		}

		virtual void create(nanim_mesh* mesh_ptr, const struct anim_mesh_morph& anim) = 0;
		
		void set_anim_status(const struct anim_mesh_morph& anim)
		{
			m_anim_status = anim;
		}

	protected:
		struct anim_mesh_morph	m_anim_status;

		nDECLARE_VIRTUAL_CLASS(nrender_anim_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RENDER_ANIM_MESH_H_