#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_nresource : nresource, boost::python::wrapper<nresource>
	{
		typedef boost::intrusive_ptr<py_nresource> ptr;

		explicit py_nresource(const nstring& name_str):nresource(name_str)
		{}

		bool ready() const
		{
			boost::python::override f = get_override("ready");	
			if( f )
				return f();			
			else
				return false;
		}

		void save_to_file(const resource_location& loc)
		{
			boost::python::override f = get_override("save_to_file");	
			if( f )
				f(loc);
			else
				nresource::save_to_file(loc);
		}

		void save_to_file_default(const resource_location& loc)
		{
			nresource::save_to_file(loc);
		}
	};
}


void export_nresource()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<resource_location>("resource_location")
		.def( python::init<nstring>() )
		.def( python::init<nstring, nstring>() )
		.def_readwrite("pkg_name", &resource_location::pkg_name)
		.def_readwrite("file_name", &resource_location::file_name)
		.def("to_string", &resource_location::to_string)
		.def("is_valid", &resource_location::is_valid)
		;

	class_<py_nresource,
		boost::python::bases<nobject>, 
		py_nresource::ptr,
		boost::noncopyable>("nresource", python::init<nstring>())
		.def("get_location", &nresource::get_location, python::return_value_policy<copy_const_reference>())
		.def("get_name_str", &nresource::get_name_str, python::return_value_policy<copy_const_reference>())
		//-- python使用smart_ptr，所以不需要手动处理引用计数
		//.def("add_ref", &nresource::add_ref)
		//.def("release", &nresource::release)
		.def("save_to_file", &nresource::save_to_file, &py_nresource::save_to_file_default)
		;

	python::implicitly_convertible<py_nresource::ptr, nresource::ptr>();
}