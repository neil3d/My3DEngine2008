#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_light_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nlight_component,
		boost::python::bases<nactor_component>, 		
		boost::noncopyable >
		("nlight_component", python::init<nstring>())
		.def("create_point_light", &nlight_component::create_point_light)
		.def("create_directional_light", &nlight_component::create_directional_light)
		.def("get_point_light", &nlight_component::get_point_light, return_value_policy<reference_existing_object>())
		.def("get_directional_light", &nlight_component::get_directional_light, return_value_policy<reference_existing_object>())
		;
}