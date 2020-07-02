#include "ncommon.h"
#include "ngm_commands.h"
#include "nclient_session.h"
#include "memory/nmemory.h"
#include "nlog.h"

#include <boost/test/unit_test.hpp>

using namespace nexus;

BOOST_AUTO_TEST_CASE(test_gm_command)
{
	ngm_commands test_command;
	test_command.init();

	nclient_session session;
	session.init(0,0,0,5,0,0,_T("test"),0);

	test_command.parse_commands(_T(".test, 10001, 10, 0.12, name"), &session);
	test_command.parse_commands(_T(".world_test	10001	 	10 0.12"), &session);
	test_command.parse_commands(_T(".test	10001	 	10 0.12"), &session);

	test_command.parse_commands(_T("test 10001 10"), &session);
	test_command.parse_commands(_T(".test 10001 10"), &session);
	test_command.parse_commands(_T("..test 10001 10"), &session);
	test_command.parse_commands(_T("!test 10001 10"), &session);
	test_command.parse_commands(_T("!!test 10001 10"), &session);
	test_command.parse_commands(_T("test 10001 10"), NULL);
}
