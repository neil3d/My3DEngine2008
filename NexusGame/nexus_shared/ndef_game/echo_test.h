#ifndef _ECHO_TEST_H_
#define _ECHO_TEST_H_

#include "nprotocol_base.h"

using namespace nexus;

#pragma pack(push,1)

enum ENetMsg
{
	NI_Connect			= 1,
	NI_Disconnect		= 2,

	NC_Chat				= 11,
	NS_Chat				= 12,

	NC_MoveTest			= 13,
	NS_MoveTest			= 14,

	NC_LoadTest			= 15,
	NS_LoadTest			= 16,

	NC_CmdTest			= 17,
	NS_CmdTest			= 18,
};

CMD_START(NI_Connect)
	uint32 address;
	uint32 port;
CMD_END

CMD_START(NI_Disconnect)
CMD_END

CMD_START(NS_Chat)
	uint16	len;
	char	msg[300];
CMD_END

CMD_START(NC_Chat)
	uint16	len;
	char	msg[256];
CMD_END

struct vector3
{
	float x, y, z;

	vector3() : x(0.0f), y(0.0f), z(0.0f) {}
};

CMD_START(NC_MoveTest)
	vector3	src;
	vector3	dir;
	uint32	flag;
	uint32	flag2;
	uint32	time_stamp;
CMD_END

CMD_START(NS_MoveTest)
	uint64	player_id;
	vector3	src;
	vector3	dir;
	uint32	flag;
	uint32	flag2;
	uint32	time_stamp;
CMD_END

CMD_START(NC_LoadTest)
	uint32	time_stamp;
CMD_END

CMD_START(NS_LoadTest)
	uint32	time_stamp;
	uint32	data[250];
CMD_END

CMD_START(NC_CmdTest)
	uint64	player_id;
	uint64	item_id;
	uint32	time_stamp;
CMD_END

CMD_START(NS_CmdTest)
	uint32	time_stamp;
CMD_END

#pragma pack(pop)

#endif // _ECHO_TEST_H_
