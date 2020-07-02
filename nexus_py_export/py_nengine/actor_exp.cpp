#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_actor : nactor, boost::python::wrapper<nactor>
	{
	};
}

void export_actor()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<py_actor,
		boost::python::bases<nevent_target>, 
		boost::shared_ptr<nactor>,
		boost::noncopyable >
		("nactor", python::init<nstring>())
		.def("create_component", &nactor::create_component)
		;

}