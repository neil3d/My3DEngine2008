/**
 *	nexus ndb - ncfg
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDB_NCFG_H_
#define _NDB_NCFG_H_

#include "nconfig.h"
#include "nsingleton.h"

namespace nexus {

	enum EConfigIndex
	{
		ECI_None			= 0,
		ECI_WorldIP,
		ECI_WorldPort,
		ECI_MaxLanSession,
		ECI_DatabaseHost,
		ECI_DatabasePort,
		ECI_DatabaseName,
		ECI_Max,
	};

	/**
	 *	ncfg
	 */
	class ncfg : public nconfig
	{
	public:
		ncfg();
		~ncfg();

		virtual bool init(const tstring& file_name, uint16 values_count);
		virtual void destroy();
	};

	#define sncfg nsingleton<ncfg>::instance()

} // namespace nexus

#endif // _NDB_NCFG_H_
