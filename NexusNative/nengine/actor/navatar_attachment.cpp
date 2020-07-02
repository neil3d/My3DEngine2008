#include "StdAfx.h"
#include "navatar_attachment.h"
#include "navatar_actor.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(navatar_attachment, nstatic_mesh_component)

	navatar_attachment::navatar_attachment(const nstring& name_str) : nstatic_mesh_component(name_str)
	{
		m_bone_index = -1;
		mat_set_identity(m_ref_base);
	}

	navatar_attachment::~navatar_attachment(void)
	{
	}

	void navatar_attachment::bind_to_bone(const nstring& bone_name, const nskeleton_define* skel)
	{
		m_bone_name = bone_name;
		m_bone_index = -1;

		if( skel )
		{
			for (size_t i=0; i<skel->m_bones.size(); i++)
			{
				if(bone_name == skel->m_bones[i].name)
				{
					m_bone_index = i;
					m_ref_base = skel->m_ref_base[i];
					break;
				}
			}
		}
	}

	void navatar_attachment::update(float delta_time, const nviewport& view)
	{
		nstatic_mesh_component::update(delta_time, view);

		if( m_bone_index != -1)
		{
			navatar_actor* avatar = (navatar_actor*)m_owner;
			nskel_anim_controller::ptr anim_ctrl = avatar->get_anim_ctrl();
			const matrix_palette& mat_pal = anim_ctrl->get_matrix_palette();

			m_mat_world = m_ref_base*mat_pal[m_bone_index]*avatar->get_world_matrix();
		}
	}
}//namespace nexus