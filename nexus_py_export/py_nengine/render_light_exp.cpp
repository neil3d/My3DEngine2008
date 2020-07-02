#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_render_light()
{
	using namespace boost::python;
	using namespace nexus;	

	enum_<ELightType>("ELightType")
		.value("ELight_None", ELight_None)
		.value("ELight_Point", ELight_Point)
		.value("ELight_Directional", ELight_Directional)		
		.export_values()
		;

	python::register_ptr_to_python<nrender_light_proxy*>();
	class_<nrender_light_proxy>("nrender_light_proxy")
		.def_readwrite("m_type",&nrender_light_proxy::m_type)
		.def_readwrite("m_ambient_color",&nrender_light_proxy::m_ambient_color)
		.def_readwrite("m_diffuse_color",&nrender_light_proxy::m_diffuse_color)
		;

	python::register_ptr_to_python<npoint_light_proxy*>();
	class_<npoint_light_proxy, boost::python::bases<nrender_light_proxy>>("npoint_light_proxy")
		.def_readwrite("m_position",&npoint_light_proxy::m_position)
		.def_readwrite("m_attenuation",&npoint_light_proxy::m_attenuation)
		;

	python::register_ptr_to_python<ndirectional_light_proxy*>();
	class_<ndirectional_light_proxy, boost::python::bases<nrender_light_proxy>>("ndirectional_light_proxy")
		.def_readwrite("m_direction",&ndirectional_light_proxy::m_direction)		
		;
}
