#include "ncore.h"
#include <boost/test/unit_test.hpp>
#include "timer/nprofile.h"

#include <iostream>
#include <sstream>
#include <windows.h>

using namespace nexus;
using namespace std;

void mySleep( double seconds )
{
	Sleep( int( seconds * 1000.0 ) );
}

BOOST_AUTO_TEST_CASE(test_profile_counter)
{
	unsigned int test_count = 20;
	unsigned int count = 3;

	for (unsigned int j=0; j<test_count; ++j)
	{
		for (unsigned int i=0; i<count; ++i)
		{
			std::wostringstream name;
			name << _T("counter") << i;
			
			START_PROFILE_COUNTER( name.str() );
			mySleep(0.001);
			END_PROFILE_COUNTER( name.str() );
		}
	}

	for (unsigned int i=0; i<count; ++i)
	{
		std::wostringstream name;
		name << _T("counter") << i;

		profile_value* value = profile_manager::instance()->find_profile(nname(name.str()));
		std::wcout << name.str() << _T(": ") << value->to_string();
	}

	std::system("pause");
}
