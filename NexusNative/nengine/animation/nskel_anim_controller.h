#ifndef _NEXUS_SKEL_ANIM_CONTROLLER_H_
#define _NEXUS_SKEL_ANIM_CONTROLLER_H_
#include "nanim_controller.h"
#include "../resource/nresource_skeletal_anim_set.h"
#include "../mesh/skeletal_anim.h"

namespace nexus
{
	class nAPI nskel_anim_controller :
		public nanim_controller
	{
	public:
		typedef shared_ptr<nskel_anim_controller> ptr;

		nskel_anim_controller(void);
		virtual ~nskel_anim_controller(void);

		virtual void advance_anim(float delta_time) = 0;
		virtual const nskeleton_define* get_skeleton() const = 0;

		const matrix_palette& get_matrix_palette() const	{	return m_mat_palette;}

	protected:
		matrix_palette			m_mat_palette;
		std::vector<bone_atom>	m_bone_atoms;

		nDECLARE_VIRTUAL_CLASS(nskel_anim_controller)
	};
}//namespace nexus
#endif //_NEXUS_SKEL_ANIM_CONTROLLER_H_