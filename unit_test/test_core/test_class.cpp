#include "ncore.h"
#include <iostream>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace nexus;

class test_my_virtual : public nobject
{
public:
	test_my_virtual()	{}
	virtual ~test_my_virtual()	{}

	virtual void hello() = 0;

nDECLARE_VIRTUAL_CLASS(test_my_virtual);
};
nDEFINE_VIRTUAL_CLASS(test_my_virtual, nobject);

class test_my_class : public test_my_virtual
{
public:
	test_my_class()	{}
	virtual ~test_my_class()	{}

	virtual void hello()
	{
		cout << "hello" << endl;
	}

	nDECLARE_CLASS(test_my_class);
};
nDEFINE_CLASS(test_my_class, test_my_virtual);

BOOST_AUTO_TEST_CASE(test_nobject_class)
{
	const nclass* class_ptr = nclass_factory::instance()->find_class(_T("nobject"));
	BOOST_CHECK( class_ptr != NULL);

	const nclass* my_base_class = nclass_factory::instance()->find_class(_T("test_my_virtual"));
	BOOST_CHECK( my_base_class != NULL);

	const nclass* my_derived_class = nclass_factory::instance()->find_class(_T("test_my_class"));
	BOOST_CHECK( my_derived_class != NULL);
	BOOST_CHECK( my_derived_class->is_kind_of(my_base_class) );
	BOOST_CHECK( my_derived_class->is_kind_of(_T("test_my_virtual")) );

	{
		boost::shared_ptr<nobject> my_obj(my_derived_class->alloc_object());
		BOOST_CHECK(my_obj);

		boost::shared_ptr<test_my_class> my_obj_cast = boost::dynamic_pointer_cast<test_my_class>(my_obj);
		BOOST_CHECK(my_obj_cast);
	}
	
}

BOOST_AUTO_TEST_CASE(test_sub_class_array)
{
	nclass_factory::instance()->build_sub_classes();

	size_t total = nclass_factory::instance()->get_num_classes();
	size_t sub_lv1 = nobject::reflection_get_static_class()->get_num_children();
}