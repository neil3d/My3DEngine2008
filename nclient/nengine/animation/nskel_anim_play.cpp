#include "StdAfx.h"
#include "nskel_anim_play.h"

namespace nexus
{
	nDEFINE_CLASS(nskel_anim_play, nskel_anim_controller)

	nskel_anim_play::nskel_anim_play(void)
	{
		m_cur_frame = 0;
		m_next_frame = 0;
		m_frame_time = 0;
		m_playing = false;
		m_matrix_dirty = true;
		m_loop_mode = EAnim_Loop;
	}

	nskel_anim_play::~nskel_anim_play(void)
	{
	}

	void nskel_anim_play::init(nresource_skeletal_anim_set::ptr anim_set, bool auto_play)
	{
		m_anim_set = anim_set;

		const nskeleton_define& skeleton = m_anim_set->get_skeleton();
		
		m_bone_atoms.resize(skeleton.m_bones.size());
		m_mat_palette.resize(m_bone_atoms.size());

		for(size_t i=0; i<skeleton.m_bones.size(); i++)
		{
			const bone_info& bi = skeleton.m_bones[i];
			m_bone_atoms[i] = bi.pos;

			mat_set_identity(m_mat_palette[i]);
		}
		

		m_cur_sequence = m_anim_set->get_anim(0);

		update_matrix_palette();
	}

	void nskel_anim_play::advance_anim(float delta_time)
	{
		if( !m_anim_set )
			return;
		if( !m_cur_sequence )
			return;

		m_frame_time += delta_time;

		while( m_frame_time > m_cur_sequence->get_frame_time(m_cur_frame))
		{
			m_frame_time -= m_cur_sequence->get_frame_time(m_cur_frame);

			m_cur_frame++;

			if( m_cur_frame >= m_cur_sequence->get_num_frame())
				m_cur_frame = 0;

			m_matrix_dirty = true;
		}

		update_matrix_palette();
	}

	void nskel_anim_play::play_anim(const nstring& seq_name, bool loop, float start_time)
	{
		m_cur_sequence = m_anim_set->find_anim_by_name(seq_name);

		m_cur_frame = 0;
		m_frame_time = 0;

		m_loop_mode = loop?EAnim_Loop:EAnim_PlayOnce;
		m_playing = true;

		advance_anim(start_time);
	}

	void nskel_anim_play::stop_anim()
	{
		m_playing = false;
	}

	inline void _get_bone_atom_matrix(const bone_atom& bone, matrix44& bone_mat)
	{
		mat_set_identity(bone_mat);
		quat_to_matrix(bone.orientation, bone_mat);		
		mat_translate(bone_mat, bone.position.x, bone.position.y, bone.position.z);
	}

	void nskel_anim_play::update_matrix_palette()
	{
		if( !m_matrix_dirty )
			return;

		m_matrix_dirty = false;

		const nskeleton_define& skeleton = m_anim_set->get_skeleton();
		const matrix_palette& ref_base_inv = skeleton.m_ref_base_inv;

		//-- bone atom array
		for(size_t i=0; i<skeleton.m_bones.size(); i++)
		{
			bone_atom& this_bone = m_bone_atoms[i];
			const bone_track& track = m_cur_sequence->m_tracks[i];

			this_bone.position = track.pos_keys[m_cur_frame];
			this_bone.orientation = quat_normalize( track.rot_keys[m_cur_frame] );

		}

		//-- matrix palete
		matrix44& root_bone_mat = m_mat_palette[0];
		const bone_atom& root_bone = m_bone_atoms[0];

		root_bone.get_matrix( root_bone_mat );

		for(size_t i=1; i<skeleton.m_bones.size(); i++)
		{
			const bone_info& bi = skeleton.m_bones[i];
			const bone_atom& this_bone = m_bone_atoms[i];
			
			const matrix44& parent_mat = m_mat_palette[bi.parent_index];

			matrix44& bone_mat = m_mat_palette[i];
			this_bone.get_matrix( bone_mat );
			
			bone_mat = bone_mat*parent_mat;
		}

		root_bone_mat = ref_base_inv[0]*root_bone_mat;
		for(size_t i=1; i<skeleton.m_bones.size(); i++)
		{
			matrix44& bone_mat = m_mat_palette[i];
			bone_mat = ref_base_inv[i]*bone_mat;
		}
	}
}//namespace nexus