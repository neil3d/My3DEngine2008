#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_primitive_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nprimitive_component,
		boost::python::bases<nactor_component>, 
		boost::shared_ptr<nprimitive_component>,
		boost::noncopyable >
		("nprimitive_component", python::init<nstring>())
		;
}