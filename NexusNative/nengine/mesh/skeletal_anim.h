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

	struct nAPI nskeletal_anim_sequence
		: public nobject
	{
		typedef shared_ptr<nskeletal_anim_sequence> ptr;

		nstring	m_name;
		float	m_length;	// play back length( in seconds )
		size_t	m_num_frame;

		// one track for one bone
		std::vector<bone_track>	m_tracks;

		size_t get_num_frame() const	{	return m_tracks[0].key_times.size();}
		float get_frame_time(size_t frm) const 	{	return m_tracks[0].key_times[frm];}

		virtual void serialize(narchive& ar);

		nDECLARE_CLASS(nskeletal_anim_sequence)
	};

	struct bone_atom
	{
		vector3	position;
		quat	orientation;		

		bone_atom():
		orientation(0,0,0,0), 
		position(0,0,0)
		{}

		bone_atom(const quat& rot, const vector3& t) : 
			orientation(rot), 
			position(t)
		{}

		//explicit bone_atom(const bone_atom& other) : 
		//	orientation(other.orientation), 
		//	position(other.position), 
		//	scale(other.scale)
		//{}

		inline bone_atom& operator= (const bone_atom& ratom)
		{
			orientation = ratom.orientation;
			position = ratom.position;
			return *this;
		}

		void get_matrix(matrix44& bone_mat) const
		{
#if 0
			mat_set_identity(bone_mat);
			quat_to_matrix44(orientation, bone_mat);
			mat_translate(bone_mat, position.x, position.y, position.z);
#else
			float scale = 1.0f;
			bone_mat.m[3][0] = position.x;
			bone_mat.m[3][1] = position.y;
			bone_mat.m[3][2] = position.z;

			const float x2 = orientation.x + orientation.x;	
			const float y2 = orientation.y + orientation.y;  
			const float z2 = orientation.z + orientation.z;
			{
				const float xx2 = orientation.x * x2;
				const float yy2 = orientation.y * y2;			
				const float zz2 = orientation.z * z2;

				bone_mat.m[0][0] = (1.0f - (yy2 + zz2)) * scale;	
				bone_mat.m[1][1] = (1.0f - (xx2 + zz2)) * scale;
				bone_mat.m[2][2] = (1.0f - (xx2 + yy2)) * scale;
			}
			{
				const float yz2 = orientation.y * z2;   
				const float wx2 = orientation.w * x2;	

				bone_mat.m[2][1] = (yz2 - wx2) * scale;
				bone_mat.m[1][2] = (yz2 + wx2) * scale;
			}
			{
				const float xy2 = orientation.x * y2;
				const float wz2 = orientation.w * z2;

				bone_mat.m[1][0] = (xy2 - wz2) * scale;
				bone_mat.m[0][1] = (xy2 + wz2) * scale;
			}
			{
				const float xz2 = orientation.x * z2;
				const float wy2 = orientation.w * y2;   

				bone_mat.m[2][0] = (xz2 + wy2) * scale;
				bone_mat.m[0][2] = (xz2 - wy2) * scale;
			}

			bone_mat.m[0][3] = 0.0f;
			bone_mat.m[1][3] = 0.0f;
			bone_mat.m[2][3] = 0.0f;
			bone_mat.m[3][3] = 1.0f;
#endif
		}

		//void blend(const bone_atom& other, float alpha)
		//{
		//	if( alpha > nEPS )
		//	{
		//		if( alpha >= 1.f - nEPS )
		//		{
		//			// if blend is all the way for child2, then just copy its bone atoms
		//			(*this) = other;
		//		}
		//		else 
		//		{
		//			// simple linear interpolation for translation and scale.
		//			position = vec_lerp(position, other.position, alpha);
		//			orientation	= quat_slerp(orientation, other.orientation, alpha);

		//			// and re-normalize
		//			orientation = quat_normalize(orientation);
		//		}
		//	}
		//}
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
		bool is_valid() const
		{
			return !m_bones.empty();
		}
	};

}//namespace nexus

#endif //_NEXUS_SKELETAL_ANIM_H_