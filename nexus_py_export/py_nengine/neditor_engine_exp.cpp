#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"
#include "engine_editor.h"

namespace nexus
{
	struct py_neditor_engine : neditor_engine, boost::python::wrapper<neditor_engine>
	{
		void render_level(const nstring& lv_name, const ncamera* cam, int wnd_handle)
		{
			boost::python::override f = get_override("render_level");	
			if( f )
				f(lv_name, cam, wnd_handle);
			else
				neditor_engine::render_level(lv_name, cam, wnd_handle);
		}

		void render_level_default(const nstring& lv_name, const ncamera* cam, int wnd_handle)
		{
			neditor_engine::render_level(lv_name, cam, wnd_handle);
		}

		void update_objects(float delta_time)
		{
			boost::python::override f = get_override("update_objects");	
			if( f )
				f(delta_time);
			else
				neditor_engine::update_objects(delta_time);
		}

		void update_objects_default(float delta_time)
		{
			neditor_engine::update_objects(delta_time);
		}
	};
}

void export_editor_engine()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<py_neditor_engine, boost::python::bases<nengine>, boost::noncopyable >("neditor_engine")
		.def("render_level", &neditor_engine::render_level, &py_neditor_engine::render_level_default)
		.def("update_objects", &neditor_engine::update_objects, &py_neditor_engine::update_objects_default)
		;
}