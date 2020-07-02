#include "stdafx.h"
#include "nlevel_info_resource.h"


namespace gameframework
{


	nlevel_info_resource::nlevel_info_resource()
	{

	}

	nlevel_info_resource::~nlevel_info_resource()
	{

	}

	void nlevel_info_resource::serialize( nfile_system* fs,const nstring& pkg_name, const nstring& file_name, enum EFileMode mode, ELevelInfoType filter )
	{
		if((mode == EFileRead && !fs->file_exists(pkg_name,file_name)))
		{
			return;
		}

		narchive::ptr resource_ap = narchive::open_xml_archive(fs, mode, pkg_name, file_name);
		nstring dummy( _T("nlevel_info_resource"));
		resource_ap->object_begin(_T("nlevel_info_resource"),dummy);

		if((filter & ELIRT_StartPoint) != 0)
		{
			nSERIALIZE(*resource_ap, m_vec_start_points);
		}
		if((filter & ELIRT_Trigger) != 0)
		{
			nSERIALIZE(*resource_ap, m_vec_triggers);
		}

		resource_ap->object_end();
		resource_ap->close();
	}

	nstring nlevel_info_resource::make_unique_id( ELevelInfoType info_type )
	{
		int count = 0;
		nname ret;
		TCHAR id[512] = {0};
		switch (info_type)
		{
		case ELIRT_StartPoint:
			{
				count = m_vec_start_points.size();
				do
				{
					swprintf(id, _T("StartPoint_%d"), ++count);
					ret.reset(nstring(id));
				}
				while (find_start_point_data(ret));
			}
			break;
		case ELIRT_Trigger:
			{
				count = m_vec_triggers.size();
				do
				{
					swprintf(id, _T("Trigger_%d"), ++count);
					ret.reset(nstring(id));
				}
				while (find_trigger_data(ret));
			}
			break;
		}
		return ret.name_str;
	}

	bool nlevel_info_resource::add_info( const start_point_data& data )
	{
		// 如果ID已经存在则返回false
		if(find_start_point_data(data.name) != NULL)
			return false;

		m_vec_start_points.push_back(data);
		return true;
	}

	bool nlevel_info_resource::add_info( const trigger_data& data )
	{
		if(find_trigger_data(data.name) != NULL)
			return false;

		m_vec_triggers.push_back(data);
		return true;
	}

	void nlevel_info_resource::remove_start_point_data( const nname& id )
	{
		for (start_point_data_vec::iterator it = m_vec_start_points.begin();
			it!=m_vec_start_points.end(); ++it)
		{
			if( it->name == id)
			{
				it = m_vec_start_points.erase(it);
				break;
			}
		}
	}

	void nlevel_info_resource::remove_trigger_data( const nname& id )
	{
		for (trigger_data_vec::iterator it = m_vec_triggers.begin();
			it!=m_vec_triggers.end(); ++it)
		{
			if( it->name == id)
			{
				it = m_vec_triggers.erase(it);
				break;
			}
		}
	}

	const start_point_data*  nlevel_info_resource::find_start_point_data( const nname& id )
	{
		for (start_point_data_vec::const_iterator it = m_vec_start_points.begin();
			it!=m_vec_start_points.end(); ++it)
		{
			if( it->name == id)
			{
				return &(*it);
			}
		}
		return NULL;
	}

	const trigger_data*  nlevel_info_resource::find_trigger_data( const nname& id )
	{
		for (trigger_data_vec::const_iterator it = m_vec_triggers.begin();
			it!=m_vec_triggers.end(); ++it)
		{
			if( it->name == id)
			{
				return &(*it);
			}
		}
		return NULL;
	}

} // end of namespace gameframework