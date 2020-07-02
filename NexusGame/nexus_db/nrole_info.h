/**
 *	nexus db - nrole_info
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NROLE_INFO_H_
#define _NDB_NROLE_INFO_H_

#include "ntime_regular.h"
#include "ncharacter.h"
#include "nitem.h"
#include "nequip.h"
#include "nskill.h"

namespace nexus{

	class nrole_mgr;
	class nlan_session;

	enum ERoleStatus_Type
	{
		ERSType_None,
		ERSType_Online,		// 角色上线
		ERSType_Offline,	// 角色下线
		ERSType_Unloadcache,// 角色从缓存中心移除
	};

	/**
	 *	nrole_info
	 */
	class nrole_info : public nrole_obj_base
	{
	public:
		friend nrole_mgr;
		static nrole_info* alloc_role_info(void);
		static void free_role_info(nrole_info* info_ptr);

	public:
		nrole_info(void);
		~nrole_info(void);

		void offline_update(uint32 elapse);

		bool cache_from_db(uint64 role_id);
		void unload_cache(void);
		void destroy(void);

		nitem&	get_items(void){ return m_item; }
		nequip& get_equips(void){ return m_equip; }
		nskill&	get_skilles(void){ return m_skill; }
		ncharacter& get_character(void){ return m_character; }

		void loop(bool looped){ m_looped = looped; }
		bool loop(void){ return m_looped; }

		void set_status(ERoleStatus_Type role_status);
		uint8 get_status(void){ return m_status; }
		bool is_status(ERoleStatus_Type role_status) { return (m_status==role_status); }

		virtual void set_unload_store(bool unload_store);

		// 角色加载发送各种角色信息
		void send_load_msg(void);

		void set_lan_session(nlan_session* session){ m_session = session; }
		nlan_session* get_lan_session(void){ return m_session; }

	protected:
		void store(void);

	private:
		ncharacter			m_character;
		nitem				m_item;
		nequip				m_equip;
		nskill				m_skill;
		bool				m_looped;


		ERoleStatus_Type	m_status;
		int32				m_offline_cache_time;	//离线后缓存的时间

		nlan_session*		m_session;
	};


	typedef	boost::unordered_map<uint64, nrole_info*>	RoleInfoMap;
	typedef	RoleInfoMap::iterator						RoleInfoMapIt;

} // namespace nexus

#endif	_NDB_NROLE_INFO_H_