/**
 *	nexus ndef_cluster - nenums.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NENUMS_H_
#define _NDEF_CLUSTER_NENUMS_H_

namespace nexus {

#pragma pack(push,1)

	enum ELanSessionStatus
	{
		ELSS_None				= 0X0000,
		ELSS_Connected			= 0X0001,
		ELSS_Logined			= 0X0002,
		ELSS_Closed				= 0X0004,
		ELSS_Max
	};

	enum EServerType
	{
		EST_None		= 0,
		EST_World		= 1,
		EST_Login		= 2,
		EST_Gateway		= 3,
		EST_DB			= 4,
		EST_Bg			= 5,
		EST_Max
	};

	enum EAccountType
	{
		EAT_None				= 0,
		EAT_Trial				= 1,
		EAT_Player				= 2,
		EAT_GM_LV1				= 3,
		EAT_GM_LV2				= 4,
		EAT_GM_LV3				= 5,
		EAT_Administrator		= 6,
		EAT_Max
	};

	enum EPlayerLoadFlag
	{
		EPLF_None				= 0,
		EPLF_Att				= 0X00000001,
		EPLF_Item				= 0X00000002,
		EPLF_Spell				= 0X00000004,
		EPLF_Aura				= 0X00000008,
		EPLF_CoolDown			= 0X00000010,
		EPLF_Skills				= 0X00000020,
		EPLF_Questing			= 0X00000040,
		EPLF_QuestDone			= 0X00000080,
		EPLF_SocialList			= 0X00000100,
		EPLF_Tile				= 0X00000200,
		EPLF_Pet				= 0X00000400,
		EPLF_Reputation			= 0X00000800,
		EPLF_Mail				= 0X00001000,
		EPLF_Guild				= 0X00002000,
		EPLF_All				= EPLF_Att + EPLF_Item + EPLF_Spell + EPLF_Aura + EPLF_CoolDown + EPLF_Skills + EPLF_Questing + EPLF_QuestDone +
									EPLF_SocialList + EPLF_Tile + EPLF_Pet + EPLF_Reputation + EPLF_Mail + EPLF_Guild,
		EPLF_UpdateStat			= EPLF_CoolDown + EPLF_Skills + EPLF_Questing + EPLF_QuestDone + EPLF_SocialList + EPLF_Tile + 
									EPLF_Pet + EPLF_Reputation + EPLF_Mail + EPLF_Guild,
		EPLF_Max
	};

	enum
	{
		MAX_SERVER_NAME_LEN = 32,
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NENUMS_H_
