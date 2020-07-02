#ifndef _NEXUS_HEIGHT_MAP_UTIL_H_
#define _NEXUS_HEIGHT_MAP_UTIL_H_
#include "nheight_map.h"
#include "../framework/nengine.h"

namespace nexus
{
	template<typename T>
	void save_height_map_as_raw(const nheight_map<T>& hmap, const nstring& pkg_name, const nstring& file_name)
	{
		nfile_system* fs = nengine::instance()->get_file_sys();
		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileWrite);

		fp->write_buffer(hmap.get_raw_data(), hmap.get_data_size());
	}

	template<typename T>
	void load_height_map_as_raw(const nheight_map<T>& hmap, const nstring& pkg_name, const nstring& file_name)
	{
		// height map必须是已经创建好了的
		nASSERT( hmap.get_data_size() > 0 );

		nfile_system* fs = nengine::instance()->get_file_sys();
		size_t file_size = fs->get_file_size(pkg_name, file_name);

		nASSERT( hmap.get_data_size() == file_size );

		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileRead);
		fp->read_buffer(hmap.get_raw_data(), file_size);

	}

}//namespace nexus
#endif //_NEXUS_HEIGHT_MAP_UTIL_H_