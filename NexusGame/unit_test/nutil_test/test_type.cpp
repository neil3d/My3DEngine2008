#include "ncommon.h"
#include "nnetwork_def.h"

#include "nspell_define.h"
// #include "world/nlevel_info_define.h"

#include <boost/test/unit_test.hpp>

using namespace nexus;


struct info_data_base
{
	// ÒÔ×Ö·û´®×îÎªID
	uint32		name;

	info_data_base()
	{}

	explicit info_data_base(const uint32& id) 
		: name(id)
	{
	}

	void test(const uint32& id)
	{
	}
};

BOOST_AUTO_TEST_CASE(test_type)
{
	int64	i64_test;
	int32	i32_test;
	int16	i16_test;
	int8	i8_test;

	uint64	ui64_test;
	uint32	ui32_test;
	uint16	ui16_test;
	uint8	ui8_test;

	bool	bool_test;
	float	float_test;
	double	double_test;
	char	char_test;

	nchar	nchar_test;

	assert(8 == sizeof(int64) && 8 == sizeof(i64_test));
	assert(4 == sizeof(int32) && 4 == sizeof(i32_test));
	assert(2 == sizeof(int16) && 2 == sizeof(i16_test));
	assert(1 == sizeof(int8) && 1 == sizeof(i8_test));

	assert(8 == sizeof(uint64) && 8 == sizeof(ui64_test));
	assert(4 == sizeof(uint32) && 4 == sizeof(ui32_test));
	assert(2 == sizeof(uint16) && 2 == sizeof(ui16_test));
	assert(1 == sizeof(uint8) && 1 == sizeof(ui8_test));

	assert(1 == sizeof(bool) && 1 == sizeof(bool_test));
	assert(4 == sizeof(float) && 4 == sizeof(float_test));
	assert(8 == sizeof(double) && 8 == sizeof(double_test));
	assert(1 == sizeof(char) && 1 == sizeof(char_test));
	assert(2 == sizeof(nchar) && 2 == sizeof(nchar_test));


#ifdef _WIN64
	assert(8 == sizeof(char*) && 8 == sizeof(void*));
	assert(4 == sizeof(long) && 8 == sizeof(void*));
#else
	assert(4 == sizeof(char*) && 4 == sizeof(void*));
	assert(4 == sizeof(long) && 4 == sizeof(void*));
#endif


	float value = 0.5f;

	int32 ivalue = int32(value / 1);

	ntcp_server_config test1;
	nsend_buffer test2;
	nmessage test3;
	ivalue = sizeof(test1);
	ivalue = sizeof(test2);
	ivalue = sizeof(test3);


#if defined(WINVER) && (WINVER < 0x0500)
	DWORD			completion_key = 0;
#else
	DWORD_PTR		completion_key = 0;
#endif

	ivalue = sizeof(completion_key);


	uint64 guid = 6546464646464;

	guid = guid & 0x000000000000FFFFui64;

	uint32 spell_info_size1 = sizeof(nspell_info);
	uint32 spell_info_size2 = sizeof(nspell_effect_info);
	uint32 spell_info_size3 = sizeof(nspell_aura_info);
	uint32 spell_info_size4 = sizeof(nname);


	
	return;
}
