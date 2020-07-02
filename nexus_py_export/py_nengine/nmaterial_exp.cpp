#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"

namespace nexus
{
	struct py_nmaterial_base : nmaterial_base,
		boost::python::wrapper<nmaterial_base>
	{
		py_nmaterial_base(const nstring& name_str):nmaterial_base(name_str)
		{}

		enum ETransparentType get_trans_type() const
		{
			return get_override("get_trans_type")();
		}

		nstring get_template_name() const
		{
			return get_override("get_template_name")();
		}

		virtual const resource_location& get_template_loc() const
		{
			return get_override("get_template_loc")();
		}

		const std::string& get_shader_code() const
		{
			return get_override("get_shader_code")();
		}

		void draw_effect_param(nshading_effect* effect_ptr) const
		{
			get_override("get_shader_code")(effect_ptr);
		}
	};
}

void export_material()
{
	using namespace boost::python;
	using namespace nexus;	

	python::register_ptr_to_python<nmaterial_base*>();
	class_<py_nmaterial_base, 
		boost::python::bases<nobject>, 
		boost::shared_ptr<py_nmaterial_base>,
		boost::noncopyable >
		("nmaterial_base", python::init<nstring>())
		.def("get_template_name", pure_virtual(&nmaterial_base::get_template_name))
		.def("get_template_loc", pure_virtual(&nmaterial_base::get_template_loc), python::return_value_policy<copy_const_reference>())
		.def("get_name", &nmaterial_base::get_name)
		;

	class_<nmaterial,
		boost::python::bases<nmaterial_base>,
		nmaterial::ptr,
		boost::noncopyable >
		("nmaterial", python::init<nstring>())
		.def("create", &nmaterial::create)
		.def("get_num_parameter", &nmaterial::get_num_parameter)
		.def("get_parameter", &nmaterial::get_parameter, python::return_value_policy<return_by_value>() )
		.def("set_texture_parameter", &nmaterial::set_texture_parameter)
		.def("set_vector_parameter", &nmaterial::set_vector_parameter)
		;
}