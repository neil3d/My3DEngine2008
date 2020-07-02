#ifndef _NEXUS_CAMERA_EXP_H_
#define _NEXUS_CAMERA_EXP_H_

inline void export_camera()
{
	using namespace boost::python;
	using namespace nexus;

	//-- ncamera -----------------------------------------------
	class_<ncamera>("ncamera")
		.def("set_lookat", &ncamera::set_lookat)
		.def("set_perspective", &ncamera::set_perspective)
		/*.def("get_view", &ncamera::get_view)
		.def("get_project", &ncamera::get_project)		
		.def("get_eye_pos", &ncamera::get_eye_pos)
		.def("get_view_x", &ncamera::get_view_x)
		.def("get_view_y", &ncamera::get_view_y)
		.def("get_view_z", &ncamera::get_view_z)*/
		;

}

#endif //_NEXUS_CAMERA_EXP_H_