#include "StdAfx.h"
#include "nskel_anim_play.h"

namespace nexus
{
	nDEFINE_CLASS(nskel_anim_play, nskel_anim_controller)

	nskel_anim_play::nskel_anim_play(void)
	{
		m_rate = 1.0f;
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

		//	mat_set_identity(m_mat_palette[i]);
		}
		
		if(auto_play)
		{
			set_anim(m_anim_set->get_anim(0));
			play_anim(true, 1.0f, 0.f);
		}

		update_matrix_palette();
	}

	void nskel_anim_play::update_matrix_palette()
	{
		if( !m_matrix_dirty 
			|| !m_cur_sequence
			|| m_mat_palette.empty())
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

	void nskel_anim_play::tick_anim(float delta_time)
	{
		if( !m_anim_set )
			return;

		if( m_playing && m_cur_sequence )
		{
			// Time to move forwards by - DeltaSeconds scaled by various factors.
			const float move_deta = m_rate * delta_time;
			advance_by(move_deta, delta_time, true);
		}
	}
	void nskel_anim_play::play_anim( bool loop, float rate, float start_time )
	{
		m_frame_time = start_time;
		m_loop_mode = loop?EAnim_Loop:EAnim_PlayOnce;
		m_rate = rate;
		m_playing = true;
	}
	void nskel_anim_play::stop_anim()
	{
		m_playing = false;
	}


	void nskel_anim_play::set_anim( const nstring& sequence )
	{
		set_anim(m_anim_set->find_anim_by_name(sequence));
		if(!m_cur_sequence)
		{
			nLog_Warning(_T("Can't find animation sequene [%s]...\r\n"), sequence.c_str());
		}
	}

	void nskel_anim_play::set_anim( nskeletal_anim_sequence::ptr sequence )
	{
		m_cur_sequence = sequence;
	}
	void nskel_anim_play::set_position( float new_time, bool fire_notifies )
	{
		//const float anim_length = m_cur_sequence ? m_cur_sequence->m_length : 0.f;
		//new_time = clamp<float>(new_time, 0.f, anim_length + nEPS);

		const float delta_time = new_time - m_frame_time;
		const float move_deta = m_rate * delta_time;
		advance_by(move_deta, delta_time, fire_notifies);
	}

	float nskel_anim_play::get_time_left() const
	{
		if(m_cur_sequence)
		{
			return std::max(m_cur_sequence->m_length - m_frame_time, 0.f);
		}
		return 0.f;
	}


	float nskel_anim_play::get_anim_length() const
	{
		return m_cur_sequence ? m_cur_sequence->m_length : 0.f;
	}

	size_t nskel_anim_play::get_anim_frame() const
	{
		return m_cur_sequence ? m_cur_sequence->get_num_frame() : 0;
	}

	const TCHAR* nskel_anim_play::get_anim_playing() const
	{
		return m_cur_sequence ? m_cur_sequence->m_name.c_str() : NULL;
	}

	void nskel_anim_play::advance_by( float move_deta, float delta_time, bool fire_notifies )
	{
		if(!m_cur_sequence || move_deta == 0.f || delta_time == 0.f)
		{
			return;
		}

		if(fire_notifies && move_deta > nEPS)
		{
			issue_notifies(move_deta);
		}

		// 增加时间
		m_frame_time += move_deta;

		const float anim_length = m_cur_sequence->m_length;
		if(m_frame_time - anim_length > FLT_EPSILON)
		{
			if(m_loop_mode == EAnim_Loop)
			{
				m_frame_time = fmodf(m_frame_time, anim_length);
			}
			else
			{
				const float move_rate = move_deta / delta_time;
				const float excess_time = (m_frame_time - anim_length) / move_rate;
				m_frame_time = anim_length;
				stop_anim();

				on_anim_end(m_cur_sequence->m_name, delta_time - excess_time);
			}
		}
		else if(m_frame_time < 0.f)
		{
			if(m_loop_mode == EAnim_Loop)
			{
				m_frame_time = fmodf(m_frame_time, anim_length);
				if( m_frame_time < 0.f )
				{
					m_frame_time += anim_length;
				}
			}
			else 
			{
				// find rate of this movement.
				const float move_rate = move_deta / delta_time;

				// figure out by how much we've reached beyond beginning of animation.
				// this is useful for transitions.
				const float excess_time = m_frame_time / abs(move_rate);
				m_frame_time = anim_length;
				stop_anim();

				// notify that animation finished playing
				on_anim_end(m_cur_sequence->m_name, delta_time - excess_time);
			}
		}

		float frame_time = 0.f;
		size_t num_frame = m_cur_sequence->get_num_frame();
		size_t i = 0;
		for (; i < num_frame; i++)
		{
			frame_time += m_cur_sequence->get_frame_time(i);
			if( m_frame_time <= frame_time)
			{
				break;
			}
		}

		if(m_cur_frame != i)
		{
			m_cur_frame = i;
			m_next_frame = m_cur_frame + 1;
			if(m_next_frame == num_frame)
			{
				m_next_frame = (m_loop_mode == EAnim_Loop) ? (m_next_frame = 0) : (m_next_frame = m_cur_frame);
			}
			nASSERT(m_next_frame < num_frame);

			m_matrix_dirty = true;
			update_matrix_palette();
		}
	}

	void nskel_anim_play::issue_notifies( float delta_time )
	{
		(void)delta_time;
	}

	void nskel_anim_play::on_anim_end( const nstring& sequence, float played_time )
	{
		(void)played_time;
		nLog_Debug(_T("nskel_anim_play::on_anim_end(%s)\r\n"), sequence.c_str());
	}

}//namespace nexus