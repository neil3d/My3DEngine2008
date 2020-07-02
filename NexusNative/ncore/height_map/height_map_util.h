/**
*	Core of Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/
#ifndef _NEXUS_CORE_HEIGHT_MAP_UTIL_H_
#define _NEXUS_CORE_HEIGHT_MAP_UTIL_H_
#include "nheight_map.h"

namespace nexus
{
	template<typename T>
	void save_height_map_as_raw(nfile_system* fs,const nheight_map<T>& hmap, const nstring& pkg_name, const nstring& file_name)
	{
		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileWrite);

		fp->write_buffer(hmap.get_raw_data(), hmap.get_data_size());
	}

	// 在调用load_height_map_as_raw函数之前需要先使用nheight_map::create函数
	template<typename T>
	bool load_height_map_as_raw(nfile_system* fs,const nheight_map<T>& hmap, const nstring& pkg_name, const nstring& file_name)
	{
		if( !fs->file_exists(pkg_name,file_name) )
		{
			return false;
		}

		// height map必须是已经创建好了的
		nASSERT( hmap.get_data_size() > 0 );

		size_t file_size = fs->get_file_size(pkg_name, file_name);

		nASSERT( hmap.get_data_size() == file_size );

		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileRead);
		fp->read_buffer(hmap.get_raw_data(), file_size);
		
		return true;
	}

}//namespace nexus
#endif //_NEXUS_HEIGHT_MAP_UTIL_H_