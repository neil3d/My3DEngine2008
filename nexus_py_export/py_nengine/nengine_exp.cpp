#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_nengine : nengine, boost::python::wrapper<nengine>
	{
		void init_core()	
		{	
			boost::python::override f = get_override("init_core");	
			if( f )
				f();
			else
				nengine::init_core();
		}
		void init_core_default()	{	nengine::init_core();}

		void init_modules(const engine_config& cfg)
		{	
			boost::python::override f = get_override("init_modules");	
			if( f )
				f(cfg);
			else
				nengine::init_modules(cfg);			
		}
		void init_modules_default(const engine_config& cfg)	{	nengine::init_modules(cfg);}

		void close()
		{
			boost::python::override f = get_override("close");	
			if( f )
				f();
			else
				nengine::close();
		}
		void close_default()	{	nengine::close();}
	};

	void py_set_window_handle(engine_config& cfg, boost::python::object py_obj)
	{
		int hwnd = boost::python::extract<int>(py_obj);
		cfg.window_handle = (void*)(hwnd);
	}
}// namespace py_nexus

void export_nengine()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<render_config>("render_config")
		.def_readwrite("window_handle", &render_config::window_handle)		
		.def_readwrite("width", &render_config::width)
		.def_readwrite("height", &render_config::height)
		.def_readwrite("color_bits", &render_config::color_bits)
		.def_readwrite("bWindowed", &render_config::bWindowed)	
		.def_readwrite("bEnableHDR", &render_config::bEnableHDR);		

	class_<engine_config, boost::python::bases<render_config> >("engine_config")		
		.def_readwrite("renderer_class", &engine_config::renderer_class)
		.def_readwrite("file_system_class", &engine_config::file_system_class)		
		.def_readwrite("engine_data_pkg", &engine_config::engine_data_pkg);		

	def("py_set_window_handle", py_set_window_handle);

	
	class_<py_nengine, boost::python::bases<nobject>, boost::noncopyable >("nengine")
		.def("init_core", &nengine::init_core, &py_nengine::init_core_default)
		.def("init_modules", &nengine::init_modules, &py_nengine::init_modules_default)
		.def("close", &nengine::close, &py_nengine::close_default)		
		.def("create_level", &nengine::create_level)
		.def("free_level", &nengine::free_level)
		.def("render_attach_element", &nengine::render_attach_element)
		.def("render_detach_element", &nengine::render_detach_element)
		.def("set_render_mode", &nengine::set_render_mode)

		.def("get_file_sys", &nengine::get_file_sys, return_value_policy<reference_existing_object>())
		;
}