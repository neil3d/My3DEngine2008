/**
 *	nexus db - nequip
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NEQUIP_H_
#define _NDB_NEQUIP_H_

#include "ntime_regular.h"

namespace nexus{

	/**
	 *	nequip
	 */
	class nequip : public nrole_obj_base
	{
	public:
		nequip(void);
		~nequip(void);

		void cache_from_db(uint64 owner_id);
		void unload_cache(void);
		nequip_cache* get_data(uint64 guid);

		void insert(nequip_cache* data_ptr);
		void destroy(uint64 guid);
		void store(void);

		EquipMap& get_equips(void){ return m_equips; }

	private:
		EquipMap	m_equips;
	};


} // namespace nexus

#endif	_NDB_NEQUIP_H_