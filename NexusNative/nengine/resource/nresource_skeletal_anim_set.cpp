#include "StdAfx.h"
#include "nresource_skeletal_anim_set.h"
#include "resource_importer_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nresource_skeletal_anim_set, nresource)

	nresource_skeletal_anim_set::nresource_skeletal_anim_set(const nstring& name_str):nresource(name_str)
	{
	}

	nresource_skeletal_anim_set::~nresource_skeletal_anim_set(void)
	{
	}

	void nresource_skeletal_anim_set::import_anim_sequence(const nstring& full_path)
	{
		nskeletal_anim_importer::data imp_data;
		resource_importer_manager::instance()->import_skeletal_anim(full_path, imp_data);

		//-- 使用第一个导入的PSA文件的skeleton定义，这要求PSK文件也是使用这个动作导出的
		if( m_skeleton.is_valid() )
		{
			// 比较骨架结构必须一致
			const nskeleton_define& sk1 = m_skeleton;
			const nskeleton_define& sk2 = imp_data.skeleton;
			if(sk1.m_bones.size() == sk2.m_bones.size())
			{
				size_t s = sk1.m_bones.size();
				for (int i=0; i< s; i++)
				{
					const bone_info& b1 = sk1.m_bones[i];
					const bone_info& b2 = sk2.m_bones[i];

					if( b1.name!=b2.name
						|| b1.num_children!=b2.num_children
						|| b1.parent_index!=b2.parent_index)
						nthrow2(_T("Skeleton does NOT Match!"), full_path.c_str());

				}
			}
			else
			{
				nthrow2(_T("Skeleton does NOT Match!"), full_path.c_str());
			}
				
		}
		else
		{
			m_skeleton = imp_data.skeleton;
		}
		
		//-- 新导入的动作加在数组后面
		for(size_t i=0; i<imp_data.anim_array.size(); i++)
		{
			m_anim_array.push_back( imp_data.anim_array[i] );
		}
	}

	nskeletal_anim_sequence::ptr nresource_skeletal_anim_set::find_anim_by_name(const nstring& name_str) const
	{
		nskeletal_anim_sequence::ptr ret;

		for(sequence_array::const_iterator iter = m_anim_array.begin();
			iter != m_anim_array.end();
			++ iter)
		{
			if( (*iter)->m_name == name_str )
			{
				ret = (*iter);
				break;
			}
		}

		return ret;
	}

	template<>
	inline narchive& nserialize(narchive& ar, bone_atom& ba, const TCHAR* obj_name)
	{
		nstring class_name(_T("bone_atom"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, ba.position, _T("position"));
		nserialize(ar, ba.orientation, _T("orientation"));
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, bone_info& bi, const TCHAR* obj_name)
	{
		nstring class_name(_T("bone_info"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, bi.name, _T("name"));
		nserialize(ar, bi.num_children, _T("num_children"));
		nserialize(ar, bi.parent_index, _T("parent_index"));
		nserialize(ar, bi.pos, _T("pos"));		
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, nskeleton_define& skl, const TCHAR* obj_name)
	{
		nstring class_name(_T("nskeleton_define"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, skl.m_bones, _T("m_bones"));
		nserialize(ar, skl.m_ref_base, _T("m_ref_base"));		
		ar.object_end();

		if(ar.is_loading())
			skl.init_ref_base();
		return ar;
	}

	void nresource_skeletal_anim_set::serialize(narchive& ar)	
	{
		nresource::serialize(ar);

		nSERIALIZE(ar, m_skeleton);
		nSERIALIZE(ar, m_anim_array);
	}
}//namespace nexus