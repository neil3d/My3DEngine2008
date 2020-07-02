#include "nlzo_wrap.h"
#include "nmsg_movement.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace nexus;

BOOST_AUTO_TEST_CASE(test_lzo)
{
	{
		nlzo_wrap lzo;

		if (!lzo.init())
		{
			std::cout << "lzo init error!" << std::endl;
			return;
		}

		tagS2C_PlayerEnterAoi msg1;

		msg1.player_id		= 9713131346463131;
		msg1.position		= vector3(10.1f, 451.5f,62.f);
		msg1.yaw			= 3.14f;
		msg1.acceleration	= vector3(12.1f, 551.5f,67.f);
		msg1.time_stamp		= 64631.0f;
		msg1.flag			= 5;



		tagS2C_PlayerEnterAoi msg2;

		msg1.player_id		= 786113164646464;
		msg1.position		= vector3(90.1f, 141.5f,63.f);
		msg1.yaw			= 3.14f;
		msg1.acceleration	= vector3(121.1f, 5151.5f,267.f);
		msg1.time_stamp		= 62141.0f;
		msg1.flag			= 0;


		tagS2C_PlayerEnterAoi msg3;

		msg1.player_id		= 4510220;
		msg1.position		= vector3(103.1f, 4651.5f,6672.f);
		msg1.yaw			= 3.14f;
		msg1.acceleration	= vector3(142.1f, 5551.5f,7767.f);
		msg1.time_stamp		= 231.0f;
		msg1.flag			= 1;


		char in[214];
		uint32 in_len = 0;

		memcpy(in + in_len, &msg1, sizeof(msg1));
		in_len += sizeof(msg1);

		memcpy(in + in_len, &msg2, sizeof(msg2));
		in_len += sizeof(msg2);

		memcpy(in + in_len, &msg3, sizeof(msg3));
		in_len += sizeof(msg3);

		uint32 out_len = sizeof(lzo_uint);
		char out[2140];


		lzo.compress(in, in_len, out, out_len);


		char out2[1500];
		uint32 out_len2 = 0;

		lzo.decompress(out, out_len, out2, out_len2);

		assert(out_len2 == in_len);

		if (out_len2 != in_len)
		{
			std::cout << "lzo decompress error!" << std::endl;
		}

		for (uint32 i = 0; i < in_len; ++i)
		{
			assert(in[i] == out2[i]);


			if (in[i] != out2[i])
			{
				std::cout << "lzo decompress error!" << std::endl;
			}

		}

		lzo.destroy();


	}

	return;
}
