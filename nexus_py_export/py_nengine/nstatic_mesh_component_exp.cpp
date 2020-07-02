#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_static_mesh_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nstatic_mesh_component,
		boost::python::bases<nmesh_component>, 
		boost::shared_ptr<nstatic_mesh_component>,
		boost::noncopyable >
		("nstatic_mesh_component", python::init<nstring>())
		.def("reset_resource", &nstatic_mesh_component::reset_resource)
		;
}