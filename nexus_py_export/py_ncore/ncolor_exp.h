#ifndef _NEXUS_COLOR_EXP_H_
#define _NEXUS_COLOR_EXP_H_

inline void export_color()
{
	using namespace boost::python;
	using namespace nexus;

	//-- color4f -----------------------------------------------
	class_<color4f>("color4f")
		.def( boost::python::init<float, float, float, float>() )
		.def_readwrite("R", &color4f::R)
		.def_readwrite("G", &color4f::G)
		.def_readwrite("B", &color4f::B)
		.def_readwrite("A", &color4f::A)
		;

	//-- color4f -----------------------------------------------
	class_<color4ub>("color4ub")
		.def( boost::python::init<unsigned char, unsigned char, unsigned char, unsigned char>() )
		.def_readwrite("R", &color4ub::R)
		.def_readwrite("G", &color4ub::G)
		.def_readwrite("B", &color4ub::B)
		.def_readwrite("A", &color4ub::A)
		;

}

#endif //_NEXUS_COLOR_EXP_H_