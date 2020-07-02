/**
 *	nexus ngame - ncfg
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NCFG_H_
#define _NGAME_NCFG_H_

#include "nconfig.h"
#include "nsingleton.h"

namespace nexus {

	enum EConfigIndex
	{
		ECI_None					= 0,
		ECI_WorldPort,							// world游戏世界端口
		ECI_MaxLanSessionClient,				// lan client的最大连接数
		ECI_MaxLanSessionServer,				// lan server的最大连接数
		ECI_NLoginIp,							// login server的ip地址
		ECI_NLoginPort,							// login server的端口
		ECI_NBgIp,								// bg server的ip地址
		ECI_NBgPort,							// bg server的端口
		ECI_AeraId,								// 世界服务器所属大区id(<0xff)
		ECI_BattleGroundId,						// 世界服务器所属战场组id(<0xff)
		ECI_WorldId,							// 战场组内游戏世界id(<0xff)
		ECI_WorldName,							// 游戏世界名称，服务器名称
		ECI_ServerId,							// 服务器id, 由计算规则得出
		ECI_ResPath,							// 服务器资源路径根目录

		ECI_FloatTest,							// float test
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

	private:
		uint32 _calculate_server_id();
	};

	#define sncfg nsingleton<ncfg>::instance()

} // namespace nexus

#endif // _NGAME_NCFG_H_
