#include <boost/python.hpp>
#include "../../ncore/file/nfile_system.h"
#include "../../ncore/file/nfile_query.h"
#include "nfile_system_exp.h"

namespace nexus
{
	struct py_file_system : nfile_system, 
		boost::python::wrapper<nfile_system>
	{
		void init(const nstring& root)	{}
		void close()	{}

		size_t get_num_package() const
		{
			return this->get_override("get_num_package")();
		}
		
		nstring get_package_name(size_t i)
		{
			return this->get_override("get_package_name")(i);
		}

		void query_package(const nstring& pkg_name, const nstring& path, nfile_query& out_info)
		{
			this->get_override("query_package")(pkg_name, path, out_info);
		}
		void open_package(const nstring& pkg_name)	{}
		void close_package(const nstring& pkg_name)	{}
		nfile::ptr open_file(const nstring& pkg_name, const nstring& file_name, enum EFileMode mode)
		{ return nfile::ptr();	}
		bool remove_file(const nstring& pkg_name, const nstring& file_name)
		{	return false; }
		int get_file_size(const nstring& pkg_name, const nstring& file_name)
		{	return -1; }
		bool file_exists(const nstring& pkg_name, const nstring& file_name)
		{	return false; }
	};

	struct py_nfile_query : nfile_query,
		boost::python::wrapper<nfile_query>
	{
		void on_file_entity(const nfile_entity& file)
		{
			this->get_override("on_file_entity")(file);
		}
	};
}

using namespace boost::python;
using namespace nexus;

void export_file_system()
{
	//-- file entity
	enum_<EFileEntityType>("EFileEntityType")
		.value("EFile_Invalid", EFile_Invalid)
		.value("EFile_Regular", EFile_Regular)
		.value("EFile_Folder", EFile_Folder)		
		.export_values()
		;

	class_<nfile_entity>("nfile_entity")
		.def_readwrite("type", &nfile_entity::type)		
		.def_readwrite("name", &nfile_entity::name)		
		.def_readwrite("path", &nfile_entity::path)		
		.def_readwrite("file_size", &nfile_entity::file_size)		
		;

	//-- file query interface	
	class_<py_nfile_query, 		
		boost::noncopyable>("nfile_query")
		.def("on_file_entity", pure_virtual(&nfile_query::on_file_entity))
		;

	//-- class nfile_system
	python::register_ptr_to_python<nfile_system*>();
	class_<py_file_system, 
		boost::python::bases<nobject>,		
		boost::noncopyable >("nfile_system")
		.def("get_num_package", pure_virtual(&nfile_system::get_num_package))
		.def("get_package_name", pure_virtual(&nfile_system::get_package_name))
		.def("query_package", pure_virtual(&nfile_system::query_package))
		;
}