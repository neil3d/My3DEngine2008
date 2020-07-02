#include "StdAfx.h"
#include "nresource_skeletal_anim_set.h"
#include "resource_importer_manager.h"

namespace nexus
{
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

		m_skeleton = imp_data.skeleton;
		m_anim_array = imp_data.anim_array;
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

	void nresource_skeletal_anim_set::serialize(narchive& ar)	
	{}
}//namespace nexus