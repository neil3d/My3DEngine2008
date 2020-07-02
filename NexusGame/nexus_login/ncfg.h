/**
 *	nexus nlogin - ncfg
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NCFG_H_
#define _NLOGIN_NCFG_H_

#include "nconfig.h"
#include "nsingleton.h"

namespace nexus {

	enum EConfigIndex
	{
		ECI_None			= 0,
		ECI_LanPort,
		ECI_MaxLanSession,
		ECI_DatabaseHost,
		ECI_DatabasePort,
		ECI_DatabaseName,
		ECI_ProofLevel,
		ECI_MaxPlayerNum,
		ECI_PerPermitPlayer,
		ECI_Max,
	};


	// 账号验证等级
	enum EProofLevel
	{
		EPIType_None			= 0,
		EPIType_ProofNo			= 1,	//不需要做任何验证
		EPIType_ProofAll		= 2,	//全部验证(账号、密码、密保)	
		EPIType_Max,
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

#endif // _NLOGIN_NCFG_H_
