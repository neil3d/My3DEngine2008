/**
*	Core of Nexus Engine -  Object Name
*
*	Author: Neil
*	Date:	Dec, 2008
*/


#ifndef _NEXUS_CORE_NAME_H_
#define _NEXUS_CORE_NAME_H_
#include "serialize.h"

namespace nexus
{
	/**
	 *	对象名称
	 *	@remark 包括一个字符串和一个CRC值（用于快速比较）
	*/
	struct nCoreAPI nname
	{
		nstring			name_str;
		unsigned int	name_crc;

		nname(void):name_crc(0)
		{}

		explicit nname(const nstring& szname);
		void reset(const nstring& szname);
	};
	
	inline bool operator < (const nname& v1, const nname& v2)
	{
		return v1.name_crc < v2.name_crc;
	}

	inline bool operator == (const nname& v1, const nname& v2)
	{
		return v1.name_crc == v2.name_crc;
	}

	template<>
	inline narchive& nserialize(narchive& ar, nname& obj, const TCHAR* obj_name)
	{
		nserialize(ar, obj.name_str, obj_name);
		if( ar.is_loading() )
			obj.reset(obj.name_str);
		return ar;
	}
}

#endif