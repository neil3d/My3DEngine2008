#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_nresource_static_mesh()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<nresource_static_mesh,
		boost::python::bases<nresource>, 
		nresource_static_mesh::ptr,
		boost::noncopyable>("nresource_static_mesh", python::init<nstring>())
		.def("import_lod", &nresource_static_mesh::import_lod)
		.def("import_set_material", &nresource_static_mesh::import_set_material)
		.def("get_num_lod", &nresource_static_mesh::get_num_lod)
		.def("get_num_section", &nresource_static_mesh::get_num_section)
		.def("get_material", &nresource_static_mesh::get_material, return_value_policy<reference_existing_object>())
		//-- static methods
		.def("get_num_importer", &nresource_static_mesh::get_num_importer)
		.staticmethod("get_num_importer")
		.def("get_importer_type", &nresource_static_mesh::get_importer_type)
		.staticmethod("get_importer_type")
		.def("get_importer_desc", &nresource_static_mesh::get_importer_desc)
		.staticmethod("get_importer_desc")
		;
	python::implicitly_convertible<nresource_static_mesh::ptr, nresource::ptr>();

}