#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_material_template()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nmaterial_template,
		boost::python::bases<nresource>, 
		nmaterial_template::ptr,
		boost::noncopyable>("nmaterial_template", python::init<nstring>())
		.def("get_trans_type", &nmaterial_template::get_trans_type)
		.def("set_trans_type", &nmaterial_template::set_trans_type)
		;
}