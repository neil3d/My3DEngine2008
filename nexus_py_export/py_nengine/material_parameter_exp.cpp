#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_material_parameter()
{
	using namespace boost::python;
	using namespace nexus;	

	enum_<EMaterialParameter>("EMaterialParameter")
		.value("EMPT_None", EMPT_None)
		.value("EMPT_Float", EMPT_Float)		
		.value("EMPT_Float2", EMPT_Float2)		
		.value("EMPT_Float3", EMPT_Float3)		
		.value("EMPT_Float4", EMPT_Float4)		
		.value("EMPT_Texture", EMPT_Texture)				
		.export_values()
		;

	class_<material_parameter>("material_parameter")
		.def_readwrite("name",&material_parameter::name)
		.def_readwrite("type",&material_parameter::type)
		;
}