/**
 *	nexus db - ntime_regular
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NTIME_REGULAR_H_
#define _NDB_NTIME_REGULAR_H_

#include "ncache_utility.h"

namespace nexus{

	/**
	 *	ntime_regular : 存储的计时规则
	 */
	class ntime_regular
	{
	public:
		ntime_regular(void);
		~ntime_regular(void);

		virtual void update(uint32 elapse);

		void shut_dowm(void);

		void set_inter_time(uint32 inter_time){ m_inter_time = inter_time; }
		uint32 get_inter_time(void){ return m_inter_time;}

	protected:
		virtual	void store(void) = 0;

		void start_slice(void);
		bool end_slice(void);

	private:
		uint32	m_elapse_time;	//刷新时间
		uint32	m_inter_time;	//间隔时间
		uint32	m_slice_time;	//时间切片

	};


	enum ERoleObjCachePolicy_Type
	{
		EROCPolicyType_None,
		EROCPolicyType_Update,	//定时缓存
		EROCPolicyType_OffLine,	//下线缓存
	};

	class nrole_obj_base
	{
	public:
		nrole_obj_base(void){ _init(); }
		~nrole_obj_base(){}

		uint64 get_owner_id(void) { return m_owner_id; }
		void set_owner_id(uint64 owner_id) { m_owner_id = owner_id; }

		void set_cache_policy(ERoleObjCachePolicy_Type policy_type){ m_cache_policy_type = policy_type; }
		bool cache_policy(ERoleObjCachePolicy_Type policy_type){ return (policy_type==m_cache_policy_type); }

		virtual void set_unload_store(bool unload_store) { m_unload_store = unload_store; }
		bool get_unload_store(void){ return m_unload_store; }

	private:
		void _init(void);

	protected:
		uint64						m_owner_id;
		ERoleObjCachePolicy_Type	m_cache_policy_type;
		bool						m_unload_store;			//下线时是否需要再次存储
	};

	#define ENFORCE_SAVE_INHERIT_FROM_ROLE_OBJ	//控制是否强制执行继承之nrole_obj_base的类

} // namespace nexus

#endif	_NDB_NTIME_REGULAR_H_