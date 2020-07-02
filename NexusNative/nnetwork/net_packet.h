#ifndef _NEXUS_NET_PACKET_H_
#define _NEXUS_NET_PACKET_H_
#include <boost/shared_ptr.hpp>

namespace nexus
{
	// 广域网数据包最大的大小
	const size_t NNET_MAX_WAN_PACKET_SIZE = 2048;
	// 服务器组内部局域网数据包的最大大小
	const size_t NNET_MAX_LAN_PACKET_SIZE = 4096;

#pragma pack(push,1)
	struct nnet_packet_header
	{
		unsigned short	pkt_size;
	};

	struct nnet_msg_header
	{
		unsigned short	msg_size;
		unsigned short	msg_id;
	};
#pragma pack(pop)

	typedef boost::shared_ptr<nnet_packet_header> nnet_packet_ptr;

//-- macro for message defines
#define MSG_Begin(name) struct MSG_##name {\
	unsigned short msg_size; unsigned short msg_id;\
	MSG_##name(void)	{	msg_size=unsigned short( sizeof(*this) ); msg_id=EMSG_##name;}

#define MSG_End	};
}//namespace nexus
#endif //_NEXUS_NET_PACKET_H_