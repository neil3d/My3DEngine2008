#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_actor_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nactor_component,
		boost::python::bases<nevent_target>, 
		boost::shared_ptr<nactor_component>,
		boost::noncopyable >
		("nactor_component", python::init<nstring>())
		;
}