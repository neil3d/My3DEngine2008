/**
 *	nexus network - ntype_def.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NCORE_NTYPE_DEF_H_
#define _NCORE_NTYPE_DEF_H_

#include <tchar.h>

#pragma pack(push,1)

namespace nexus {

	/**
	 *	网络协议中基本类型
	 */
	typedef signed __int64		int64;
	typedef signed __int32		int32;
	typedef signed __int16		int16;
	typedef signed __int8		int8;

	typedef unsigned __int64	uint64;
	typedef unsigned __int32	uint32;
	typedef unsigned __int16	uint16;
	typedef unsigned __int8		uint8;

	// bool类型					bool
	// 32位浮点类型			float
	// 64位浮点类型			double
	// 字符类型					char			// 一般用于定义缓冲区
	typedef TCHAR				nchar;			// 一般用于定义名称

} // namespace nexus

#pragma pack(pop)

#endif // _NNETWORK_NTYPE_DEF_H_
