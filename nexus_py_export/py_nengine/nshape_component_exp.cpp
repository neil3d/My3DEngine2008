#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_shape_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nshape_component,
		boost::python::bases<nmesh_component>, 
		boost::shared_ptr<nshape_component>,
		boost::noncopyable >
		("nshape_component", python::init<nstring>())
		.def("create_box", &nshape_component::create_box)
		.def("create_sphere", &nshape_component::create_sphere)
		.def("set_material", &nshape_component::set_material)
		;
}