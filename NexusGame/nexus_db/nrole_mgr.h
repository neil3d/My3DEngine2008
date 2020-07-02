/**
 *	nexus db - nrole_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NROLE_MGR_H_
#define _NDB_NROLE_MGR_H_

#include "ntime_regular.h"
#include "nrole_info.h"

namespace nexus{

	/**
	 *	nrole_mgr
	 */
	class nrole_mgr : public ntime_regular
	{
	public:
		nrole_mgr(void);
		~nrole_mgr(void);

		virtual void update(uint32 elapse);

		void unload_cache(void);

		// 角色上线加载游戏数据
		bool role_online(uint64 role_id, nlan_session* session);

		// 角色下线移除游戏数据
		bool role_offline(uint64 role_id);

		// 创建角色
		void create_role(ncharacter_cache* data_ptr);

		// 删除角色
		void delete_role(uint64 role_id);

		nrole_info* get_role_info(uint64 role_id);

		// 检测角色是否在线
		bool is_online(uint64 role_id);

		RoleInfoMap& get_role_map(void){ return m_role_infoes; }

		ncharacter_cache* get_character_cache(uint64 role_id);

		//角色等待加载
		void add_waiting_load_role(uint64 role_id);

		//获取当前等待加载的数量
		uint32 get_waiting_load_roles(void);

	protected:
		virtual void store(void);

	private:
		RoleInfoMap	m_role_infoes;

		Uint64List	m_load_role_list;	//请求加载的role列表
	};


} // namespace nexus

#endif	_NDB_NROLE_MGR_H_