#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_nmesh_component : nmesh_component,
		boost::python::wrapper<nmesh_component>
	{
		py_nmesh_component(const nstring& name_str):nmesh_component(name_str)
		{}

		nrender_mesh* get_render_mesh(int lod)
		{
			boost::python::override f = get_override("get_render_mesh");	
			if( f )
				return f(lod);			
			return NULL;
		}
	};
}

void export_mesh_component()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<py_nmesh_component,
		boost::python::bases<nprimitive_component>, 		
		boost::noncopyable >
		("nmesh_component", python::init<nstring>())
		;
}