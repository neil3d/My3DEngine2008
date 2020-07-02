#include "ncommon.h"

#include "nworld_object.h"
#include "nobject_data.h"

#include "nmsg_world_gateway.h"
#include "nmsg_chat.h"
#include "nmsg_loading.h"
#include "nmsg_stat.h"
#include "nconfig.h"

#include <boost/test/unit_test.hpp>

using namespace nexus;

BOOST_AUTO_TEST_CASE(test_protocol)
{
	tagS2C_ChatMessage reply;
	reply.player_id	= 1002;
	reply.type		= 17;
	reply.flag		= 0;
	reply.msg_size	= uint8(_tcslen(_T("broadcast test")) * sizeof(nchar));
	memcpy(reply.msg1.msg, _T("broadcast test"), reply.msg_size);

	tagMsg_G2W_Broadcast	test_msg;
	test_msg.push_msg(&reply, reply.get_packet_size());

	// ÐÞÕýmsg id
	test_msg.set_original_msg_id(reply.msg_id);
	test_msg.msg_id = Msg_G2W_Broadcast;
	test_msg.push_client(1000);
	test_msg.push_client(1001);
	test_msg.push_client(1002);
	test_msg.push_client(1003);
	test_msg.push_client(1004);


	tagMsg_G2W_Broadcast* packet = reinterpret_cast<tagMsg_G2W_Broadcast*>(&test_msg);
	// ÐÞ¸´msg_id
	uint16 msg_id = packet->get_original_msg_id();
	uint16 num = packet->get_client_num();
	uint32 msg_size = packet->get_msg_size();
	uint16 packet_size = packet->get_packet_size();

	assert(msg_size == reply.get_packet_size());
	assert(msg_id == S2C_ChatMessage);

	uint32 client_id = 0;

	for (uint16 i = 0; i < num; ++i)
	{
		client_id = packet->get_client_id(i);

		client_id = 0;
	}

	return;
}

BOOST_AUTO_TEST_CASE(test_msg)
{
	tagC2S_GetInitStat request1;
	tagS2C_GetInitStat reply1;
	tagC2S_GetInitSpell request2;
	tagS2C_GetInitSpell reply2;
	tagC2S_GetInitAura request3;
	tagS2C_GetInitAura reply3;

	tagS2C_StatUpdate test1;
	tagS2C_RemoteStatUpdate test2;

	uint64 uint64_ptr = 0;
	nobject_data	obj_data;
	obj_data.init((nworld_object*)(&uint64_ptr), EPF_PlayerEnd);

	obj_data.set_uint32_value(EUF_UnitMaxHealth, 1000);
	obj_data.set_uint32_value(EUF_UnitMaxEnergy, 500);
	obj_data.set_uint32_value(EUF_UnitMaxMana, 2000);
	obj_data.set_float_value(EUF_UnitSpeed, 10.55f);
	obj_data.set_float_value(EUF_UnitAttackSpeed, 20.15f);
	obj_data.set_uint32_value(EUF_UnitHealth, 1000);
	obj_data.set_uint32_value(EUF_UnitEnergy, 500);
	obj_data.set_uint32_value(EUF_UnitMana, 2000);
	obj_data.set_uint32_value(EUF_UnitLevel, 100);

	obj_data.test_send_recv();

}


BOOST_AUTO_TEST_CASE(test_op)
{

	nexus::nconfig_file test_conf;
	test_conf.open(_T("mangosd.cfg"));

	tstring str_test;
	bool bool_test = false;
	int32 int32_test = 0;
	uint32 uint32_test = 0;
	int64 int64_test = 0;
	float float_test = 0.0f;

	str_test = test_conf.get_tstring_default(_T("str_test"), _T("hello nexus, »¶Ó­À´µ½nexus world"));


	std::string ip = test_conf.get_string_default(_T("ip"), "172.0.0.1");

	int32_test = test_conf.get_int32_default(_T("int32_test"), -2000);
	uint32_test = test_conf.get_uint32_default(_T("uint32_test"), 268435455);
	int64_test = test_conf.get_int64_default(_T("int64_test"), 1152921504606846975);
	float_test = test_conf.get_float_default(_T("float_test"), 100.0520f);
	bool_test = test_conf.get_bool_default(_T("bool_test"), true);
			return;
		}

