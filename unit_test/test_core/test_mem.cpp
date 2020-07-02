#include "ncore.h"
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace nexus;

class MyTestClass
{
public:
	MyTestClass(int a)
	{}
};

BOOST_AUTO_TEST_CASE(test_new)
{
	//--
	void* p = nMalloc(16);
	nFree(p);

	int* a = nNew int[10];
	delete[] a;

	int* b = nNew int;
	delete b;

	MyTestClass* obj = nNew MyTestClass(123);
	delete obj;
}