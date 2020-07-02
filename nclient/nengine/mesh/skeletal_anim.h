#ifndef _NEXUS_SKELETAL_ANIM_H_
#define _NEXUS_SKELETAL_ANIM_H_

#include "ncore.h"

namespace nexus
{
	typedef std::vector<matrix44> matrix_palette;

	struct bone_track
	{
		std::vector<vector3>	pos_keys;
		std::vector<quat>		rot_keys;
		std::vector<float>		key_times;
	};

	struct nskeletal_anim_sequence
	{
		typedef shared_ptr<nskeletal_anim_sequence> ptr;

		nstring	m_name;
		float	m_length;	// play back lenght( in seconds )
		size_t	m_num_frame;

		// one track for one bone
		std::vector<bone_track>	m_tracks;

		size_t get_num_frame() const	{	return m_tracks[0].key_times.size();}
		float get_frame_time(size_t frm) const 	{	return m_tracks[0].key_times[frm];}
	};

	struct bone_atom
	{
		vector3	position;
		quat	orientation;		

		void get_matrix(matrix44& bone_mat) const
		{
			mat_set_identity(bone_mat);
			quat_to_matrix(orientation, bone_mat);
			mat_translate(bone_mat, position.x, position.y, position.z);
		}
	};

	struct bone_info
	{
		nstring		name;
		int			num_children;	
		int			parent_index;	// -1 for root bone
		bone_atom	pos;			// reference position
		
	};

	struct nAPI nskeleton_define
	{
		std::vector<bone_info>	m_bones;	// [0] is root bone
		matrix_palette			m_ref_base;
		matrix_palette			m_ref_base_inv;

		void init_ref_base();
	};

}//namespace nexus

#endif //_NEXUS_SKELETAL_ANIM_H_