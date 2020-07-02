#include "StdAfx.h"
#include "skeletal_anim.h"

namespace nexus
{
	nDEFINE_CLASS(nskeletal_anim_sequence, nobject)

	void nskeleton_define::init_ref_base()
	{
		m_ref_base.resize(m_bones.size());
		m_ref_base_inv.resize(m_bones.size());

		for(size_t i=0; i<m_bones.size(); i++)
		{
			const bone_info& bi = m_bones[i];
			if(i == 0)	// root bone
			{
				bi.pos.get_matrix( m_ref_base[i] );
			}
			else
			{
				const matrix44& parent_mat = m_ref_base[bi.parent_index];
				matrix44& bone_mat = m_ref_base[i];
				bi.pos.get_matrix( bone_mat );

				bone_mat = bone_mat*parent_mat;
			}

			m_ref_base_inv[i] = mat_inverse( m_ref_base[i] );
		}
	}

	template<>
	inline narchive& nserialize(narchive& ar, bone_track& bt, const TCHAR* obj_name)
	{
		nstring class_name(_T("bone_track"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, bt.pos_keys, _T("pos_keys"));
		nserialize(ar, bt.rot_keys, _T("rot_keys"));
		nserialize(ar, bt.key_times, _T("key_times"));
		ar.object_end();

		return ar;
	}


	void nskeletal_anim_sequence::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_name);
		nSERIALIZE(ar, m_length);
		nSERIALIZE(ar, m_num_frame);
		nSERIALIZE(ar, m_tracks);
	}
}//namespace nexus