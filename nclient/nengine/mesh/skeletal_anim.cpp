#include "StdAfx.h"
#include "skeletal_anim.h"

namespace nexus
{
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

}//namespace nexus