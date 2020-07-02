#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_event_target : nevent_target, boost::python::wrapper<nevent_target>
	{
		void on_event(nevent_base& evt)
		{
			boost::python::override f = get_override("on_event");	
			if( f )
				f(evt);			
		}
	};
}

void export_event()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nevent_base>("nevent_base")
		.def_readwrite("m_event_id", &nevent_base::m_event_id)
		;

	class_<py_event_target, boost::noncopyable >("nevent_target")
		.def("on_event", pure_virtual(&nevent_target::on_event))
		;
}