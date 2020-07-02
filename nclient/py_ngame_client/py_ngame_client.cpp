// py_ngame_client.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "../../nexus_py_export/py_ncore/py_ncore.h"
#include "../../nexus_py_export/py_nengine/py_nengine.h"

BOOST_PYTHON_MODULE(py_ngame_client)
{
	py_ncore_export();
	py_nengine_export();
}