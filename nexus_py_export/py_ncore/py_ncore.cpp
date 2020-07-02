#include <boost/python.hpp>
#include "../../ncore/ncore.h"
#include "py_ncore.h"

#include "nmath_exp.h"
#include "ncolor_exp.h"
#include "ncamera_exp.h"
#include "nobject_exp.h"
#include "nfile_system_exp.h"

void py_ncore_export()
{
	export_object();
	
	export_math();
	export_color();
	export_camera();

	export_file_system();
}