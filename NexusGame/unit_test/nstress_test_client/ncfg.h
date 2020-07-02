/**
 *	nexus network - ncfg
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NCFG_H_
#define _NGATEWAY_NCFG_H_

#include "nconfig.h"
#include "nsingleton.h"

namespace nexus {

	enum EConfigIndex
	{
		ECI_None			= 0,
		ECI_ServerIp,
		ECI_MaxSession,
		ECI_MsgType,
		ECI_Fps,
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

#endif // _NGATEWAY_NCFG_H_
