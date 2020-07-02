#ifndef _NEXUS_TERRAIN_EXP_H_
#define _NEXUS_TERRAIN_EXP_H_

inline void export_terrain()
{
	using namespace boost::python;
	using namespace nexus;	

	register_ptr_to_python<nterrain_actor::ptr>();

	class_<nterrain_actor, 
		boost::python::bases<nactor>, 		
		boost::noncopyable >("nterrain_actor", python::init<nstring>())
		.def("create", &nterrain_actor::create)
		.def("set_scale", &nterrain_actor::set_scale)
		.def("set_position", &nterrain_actor::set_position)
		.def("create_material_basic", &nterrain_actor::create_material_basic)
		;

}

#endif //#define _NEXUS_TERRAIN_EXP_H_