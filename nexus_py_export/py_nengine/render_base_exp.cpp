#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

void export_render_base()
{
	using namespace boost::python;
	using namespace nexus;	

	enum_<ERenderMode>("ERenderMode")
		.value("ERM_Wireframe", ERM_Wireframe)
		.value("ERM_Unlit", ERM_Unlit)
		.value("ERM_Lit", ERM_Lit)
		.value("ERM_LightingOnly", ERM_LightingOnly)
		.export_values()
		;
}
