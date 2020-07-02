#ifndef _NEXUS_CORE_OBJECT_EXP_H_
#define _NEXUS_CORE_OBJECT_EXP_H_

namespace nexus
{
	void exp_translate(nexception const& e)
	{
		std::string error_string = conv_string( e.what() );
		// Use the Python 'C' API to set up an exception object
		PyErr_SetString(PyExc_RuntimeError, error_string.c_str());
	}
}



inline void export_object()
{
	using namespace boost::python;
	using namespace nexus;

	register_exception_translator<nexception>(&exp_translate);


	class_<nobject, shared_ptr<nobject>, boost::noncopyable>("nobject");
}


#endif //_NEXUS_CORE_OBJECT_EXP_H_