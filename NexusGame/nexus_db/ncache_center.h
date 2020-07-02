/**
 *	nexus db - ncache_center
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCACHE_CENTER_H_
#define _NDB_NCACHE_CENTER_H_

#include "ncache_utility.h"

#include "ncharacter.h"
#include "nrole_mgr.h"

namespace nexus{

	class nlan_session;

	/**
	 *	ncache_center : 数据缓存中心
	 *	@remark : 目前是顺序加载，顺序加载启动时速度慢，需要想办法解决
	 */
	class ncache_center
	{
	public:
		friend	class nlan_session;

	public:
		ncache_center(void);
		~ncache_center(void);

		// 启动加载所有数据库中数据
		void start(void);

		// 刷新时间
		void update(uint32 elapse);

		// 关闭时将内存中的数据都存储到数据库
		void destroy(void);

		// 增加character
		void insert_character(ncharacter_cache* data_ptr);

		// 删除character
		void destroy_character(uint64 role_id);

		nrole_mgr& get_role_mgr(void) { return m_role_mgr; }

	private:
		nrole_mgr	m_role_mgr;
	};

	extern ncache_center g_cache_center;
} // namespace nexus

#endif	_NDB_NCACHE_CENTER_H_