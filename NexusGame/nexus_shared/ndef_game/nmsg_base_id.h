/**
 *	nexus ndef_game - nmsg_base_id.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_GAME_NMSG_BASE_ID_H_
#define _NDEF_GAME_NMSG_BASE_ID_H_

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, 
	 */
	enum EMsgBaseID
	{
		Msg_Invaild			= 0,

		Msg_Client_Login	= 1,
		Msg_Client_Char		= 11,
		Msg_Client_Map		= 31,
		Msg_Client_Chat		= 41,
		Msg_Client_Movement	= 51,
		Msg_Client_Loading	= 71,
		Msg_Client_Stat		= 121,
		Msg_Client_Spell	= 171,
		Msg_Client_GM		= 981,

		Msg_World_DB		= 1000,
		Msg_World_BG		= 1600,
		Msg_World_Gateway	= 1700,
		Msg_World_Login		= 1750,
		Msg_Cluster			= 1950,
		Msg_Max				= 2000,
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_BASE_ID_H_
