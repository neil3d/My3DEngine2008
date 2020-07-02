#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"


namespace nexus
{
	struct py_level : nlevel, boost::python::wrapper<nlevel>
	{
		void init(const nstring& scene_class_name)
		{
			boost::python::override f = get_override("init");	
			if( f )
				f(scene_class_name);
			else
				nlevel::init(scene_class_name);
		}

		void init_default(const nstring& scene_class_name)
		{
			nlevel::init(scene_class_name);
		}

		nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name)
		{
			boost::python::override f = get_override("create_actor");	
			if( f )
				return f(actor_name,class_name);
			else
				return nlevel::create_actor(actor_name,class_name);
		}

		nactor::ptr create_actor_default(const nstring& actor_name, const nstring& class_name)
		{
			return nlevel::create_actor(actor_name,class_name);
		}

		void free_actor(const nname& actor_name)
		{
			boost::python::override f = get_override("free_actor");	
			if( f )
				f(actor_name);
			else
				nlevel::free_actor(actor_name);
		}

		void free_actor_default(const nname& actor_name)
		{
			nlevel::free_actor(actor_name);
		}
	};
}

void export_level()
{
	using namespace boost::python;
	using namespace nexus;	

	class_<py_level, 
		boost::python::bases<nevent_target>, 
		nlevel::ptr,boost::noncopyable >("nlevel", init<nstring>())
		.def("init", &nlevel::init, &py_level::init_default)
		.def("create_actor", &nlevel::create_actor, &py_level::create_actor_default)
		.def("free_actor", &nlevel::free_actor, &py_level::free_actor_default)
		.def("clear_actor", &nlevel::clear_actor)
		;

}
