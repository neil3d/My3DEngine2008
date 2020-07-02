/**
 *	nexus db - ncharacter
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCHARACTER_H_
#define _NDB_NCHARACTER_H_

#include "ntime_regular.h"

namespace nexus{

	/**
	 *	ncharacter
	 */
	class ncharacter : public nrole_obj_base
	{
	public:
		ncharacter(void);
		~ncharacter(void);

		bool cache_from_db(uint64 owner_id);
		void unload_cache(void);

		ncharacter_cache* get_data(void);

		void insert(ncharacter_cache* data_ptr);
		void destroy(void);

		void store(void);

	private:
		ncharacter_cache* m_info;
	};


} // namespace nexus

#endif	_NDB_NCHARACTER_H_