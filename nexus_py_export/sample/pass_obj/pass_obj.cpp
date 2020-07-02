// pass_obj.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <boost/python.hpp>

using namespace std;
using namespace boost;

//--------------------------------------------------------------
// cpp classes
struct visiter
{
	virtual void do_something() = 0;
};

struct cpp_base
{
	virtual void do_something(const string& p1, visiter& v) = 0;
};

struct cpp_derived : public cpp_base
{
	virtual void do_something(const string& p1, visiter& v)
	{
		v.do_something();
	}
};

cpp_derived cpp_obj;

cpp_base* get_cpp_obj()
{
	return &cpp_obj;
}

//--------------------------------------------------------------
// export cpp classes
struct py_visiter : visiter, python::wrapper<visiter>
{
	virtual void do_something()
	{
		this->get_override("do_something")();
	}
};

struct py_base: cpp_base, python::wrapper<cpp_base>
{
	virtual void do_something(const string& p1, visiter& v)
	{
		this->get_override("do_something")(v);
	}
};

BOOST_PYTHON_MODULE(pass_obj)
{
	using namespace boost::python;

	class_<py_visiter, boost::noncopyable>("visiter")
		.def("do_something", pure_virtual(&visiter::do_something))
		;

	class_<py_base, boost::noncopyable>("cpp_base")
		.def("do_something", pure_virtual(&cpp_base::do_something))
		;

	register_ptr_to_python<cpp_base*>();
	def("get_cpp_obj", get_cpp_obj, return_value_policy<reference_existing_object>());
}