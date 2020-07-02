/**
 *	nexus db - nitem
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NITEM_H_
#define _NDB_NITEM_H_

#include "ntime_regular.h"

namespace nexus{

	/**
	 *	nitem
	 */
	class nitem : public nrole_obj_base
	{
	public:
		nitem(void);
		~nitem(void);

		void cache_from_db(uint64 owner_id);
		void unload_cache(void);
		nitem_cache* get_data(uint64 guid);

		void insert(nitem_cache* data_ptr);
		void destroy(uint64 guid);

		void store(void);

		ItemMap& get_items(void){ return m_items; }

	private:
		ItemMap	m_items;
	};


} // namespace nexus

#endif	_NDB_NITEM_H_