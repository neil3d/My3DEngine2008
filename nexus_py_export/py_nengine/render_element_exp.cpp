#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"


namespace nexus
{
	struct py_nrender_primitive_draw_interface : nrender_primitive_draw_interface, 
		boost::python::wrapper<nrender_primitive_draw_interface>
	{
		void begin_line_batch()
		{
			this->get_override("begin_line_batch")();
		}

		void draw_line(const vector3& start, const vector3& end, const color4f& color)
		{
			this->get_override("draw_line")(start, end, color);
		}

		void end_line_batch()
		{
			this->get_override("end_line_batch")();
		}

		void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color)
		{
			// not implement
		}

		void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color)
		{
			this->get_override("draw_simple_mesh")(mesh, color);
		}

		void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color)
		{
			this->get_override("draw_debug_string")(x, y, text, color);
		}
	};

	struct py_nrender_element : nrender_element, boost::python::wrapper<nrender_element>
	{
		void draw(nrender_primitive_draw_interface* PDI)
		{
			this->get_override("draw")( boost::python::ptr(PDI) );
		}
	};
}

void export_render_element()
{
	using namespace boost::python;
	using namespace nexus;	

	python::register_ptr_to_python<nexus::nrender_primitive_draw_interface*>();
	class_<py_nrender_primitive_draw_interface, 
		boost::python::bases<nobject>, 
		boost::noncopyable >
		("nrender_primitive_draw_interface")
		.def("begin_line_batch", pure_virtual(&nrender_primitive_draw_interface::begin_line_batch))
		.def("draw_line", pure_virtual(&nrender_primitive_draw_interface::draw_line))
		.def("end_line_batch", pure_virtual(&nrender_primitive_draw_interface::end_line_batch))
		;

	python::register_ptr_to_python< boost::shared_ptr<nrender_element> >();
	class_<py_nrender_element, 
		boost::python::bases<nobject>, 		
		boost::noncopyable >
		("nrender_element")
		.def("draw", pure_virtual(&nrender_element::draw))		
		;
}