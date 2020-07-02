#ifndef _ECHO_TEST_H_
#define _ECHO_TEST_H_

#include "nprotocol_base.h"

using namespace nexus;

#pragma pack(push,1)

enum ENetMsg
{
	NCI_Connect			= 0x0003,
	NCI_Disconnect		= 0x0004,

	NCS_Chat			= 1002,
	NCC_Chat			= 1003,

	NCC_Request			= 1004,
	NCS_Reply			= 1005,

	NCC_StressTestMove	= 1006,
	NCS_StressTestMove	= 1007,
};

CMD_START(NCI_Connect)
	uint32 param1;
	uint32 param2;
CMD_END

CMD_START(NCI_Disconnect)
CMD_END

CMD_START(NCS_Chat)
	uint16	len;
	char	msg[300];
CMD_END

CMD_START(NCC_Chat)
	uint16	len;
	char	msg[256];
CMD_END

CMD_START(NCC_Request)
	uint32	time_stamp;
	int64	type_i64;
	int32	type_i32;
	int16	type_i16;
	int8	type_i8;
	uint64	type_ui64;
	uint32	type_ui32;
	uint16	type_ui16;
	uint8	type_ui8;
CMD_END

CMD_START(NCS_Reply)
	uint32	time_stamp;
	int64	type_i64;
	int32	type_i32;
	int16	type_i16;
	int8	type_i8;
	uint64	type_ui64;
	uint32	type_ui32;
	uint16	type_ui16;
	uint8	type_ui8;
CMD_END

struct vector3
{
	float x, y, z;

	vector3() : x(0.0f), y(0.0f), z(0.0f) {}
};

CMD_START(NCC_StressTestMove)
	vector3	src;
	vector3	dir;
	uint32	flag;
	uint32	flag2;
	uint32	time_stamp;
CMD_END

CMD_START(NCS_StressTestMove)
	uint64	player_id;
	vector3	src;
	vector3	dir;
	uint32	flag;
	uint32	flag2;
	uint32	time_stamp;
	uint32	session_num;
CMD_END

#pragma pack(pop)

#endif // _ECHO_TEST_H_
