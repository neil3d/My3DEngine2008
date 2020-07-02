#ifndef _NEXUS_CAMERA_CONTROLLER_EXP_H_
#define _NEXUS_CAMERA_CONTROLLER_EXP_H_

namespace nexus
{
	struct py_editor_camera_controller : editor_camera_controller,
		boost::python::wrapper<editor_camera_controller>
	{
		void update_camera(ncamera* cam)
		{
			boost::python::override f = get_override("update_camera");	
			if( f )
				f( boost::python::ptr(cam) );
			else
				editor_camera_controller::update_camera(cam);
		}

		void update_camera_default(ncamera* cam)
		{
			editor_camera_controller::update_camera(cam);
		}
	};
}

inline void export_camera_controller()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<py_editor_camera_controller,
		boost::noncopyable>("editor_camera_controller")
		.def("update_camera", &editor_camera_controller::update_camera, &py_editor_camera_controller::update_camera_default)
		.def("set_look_at", &editor_camera_controller::set_look_at)
		.def("on_mouse_left_down", &editor_camera_controller::on_mouse_left_down)
		.def("on_mouse_left_up", &editor_camera_controller::on_mouse_left_up)
		.def("on_mouse_right_down", &editor_camera_controller::on_mouse_right_down)
		.def("on_mouse_right_up", &editor_camera_controller::on_mouse_right_up)
		.def("on_mouse_wheel", &editor_camera_controller::on_mouse_wheel)
		.def("on_mouse_move", &editor_camera_controller::on_mouse_move)
		.def("set_factor", &editor_camera_controller::set_factor)
		;
}
#endif