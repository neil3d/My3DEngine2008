#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_nresource_manager()
{
	using namespace boost::python;
	using namespace nexus;	

	enum_<EResourceIOMode>("EResourceIOMode")
		.value("EIO_Auto", EIO_Auto)
		.value("EIO_Block", EIO_Block)		
		.export_values()
		;
	
	enum_<EResourceIOPriority>("EResourceIOPriority")
		.value("EIOP_Min", EIOP_Min)
		.value("EIOP_Low", EIOP_Low)		
		.value("EIOP_Normal", EIOP_Normal)		
		.value("EIOP_High", EIOP_High)		
		.value("EIOP_Max", EIOP_Max)		
		.export_values()
		;

	class_<nresource_manager,
		boost::python::bases<nresource>, 
		boost::noncopyable>("nresource_manager")
		.def("instance", &nresource_manager::instance, return_value_policy<reference_existing_object>() )
		.staticmethod("instance")
		.def("new_static_mesh", &nresource_manager::new_static_mesh)
		.def("load_static_mesh", &nresource_manager::load_static_mesh)
		.def("load_material_template_script", &nresource_manager::load_material_template_script)
		;
}