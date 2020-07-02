/**
 *	nexus db - nskill
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NSKILL_H_
#define _NDB_NSKILL_H_

#include "ntime_regular.h"

namespace nexus{

	/**
	 *	nskill
	 */
	class nskill : public nrole_obj_base
	{
	public:
		nskill(void);
		~nskill(void);

		void cache_from_db(uint64 owner_id);
		void unload_cache(void);
		nskill_cache* get_data(uint32 skill_id);

		void insert(nskill_cache* data_ptr);
		void destroy(uint32 skill_id);
		void store(void);

		SkillMap& get_skilles(void){ return m_skilles; }

	private:
		SkillMap	m_skilles;
	};


} // namespace nexus

#endif	_NDB_NSKILL_H_