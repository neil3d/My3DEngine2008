#ifndef _SAMPLE_PROTOCOL_H_
#define _SAMPLE_PROTOCOL_H_

#include "../../nnetwork/net_packet.h"

enum ESampleNetMsg
{
	EMSG_Echo = 1000,
};

#pragma pack(push,1)

MSG_Begin(Echo)
	char msg_txt[1024];
MSG_End

#pragma pack(pop)

#endif // _SAMPLE_PROTOCOL_H_