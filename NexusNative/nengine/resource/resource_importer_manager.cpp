#include "StdAfx.h"
#include "ncore.h"
#include "resource_importer_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmesh_exporter, nobject)
	nDEFINE_VIRTUAL_CLASS(nstatic_mesh_importer, nmesh_exporter)
	nDEFINE_VIRTUAL_CLASS(nanim_mesh_importer, nmesh_exporter)
	nDEFINE_VIRTUAL_CLASS(nskeletal_mesh_importer, nmesh_exporter)
	nDEFINE_VIRTUAL_CLASS(nskeletal_anim_importer, nmesh_exporter)

	resource_importer_manager::resource_importer_manager(void)
	{
	}

	resource_importer_manager::~resource_importer_manager(void)
	{
	}

	resource_importer_manager* resource_importer_manager::instance()
	{
		static resource_importer_manager g_importer_mgr;
		return &g_importer_mgr;
	}

	template<typename TData, typename TImporter>
	shared_ptr<TData> resource_importer_manager::import_resource(const nstring& full_path)
	{
		boost::filesystem::wpath fpath(full_path);

		if( !boost::filesystem::exists(fpath) 
			|| boost::filesystem::file_size(fpath) <= 0)
		{
			nthrow2(_T("mesh import failed - file not exists or empty"), full_path.c_str());
		}

		nstring ext = boost::filesystem::extension(fpath);
		boost::to_lower(ext);


		shared_ptr<TImporter> importer;
		find_importer(ext, importer);
		
		if( !importer )
		{
			nLog_Error(_T("not find mesh importer for file : %s"), full_path.c_str());
			nthrow2(_T("mesh import failed"), full_path.c_str());
		}

		shared_ptr<TData> data_ptr = importer->import_from_file(full_path);
		importer->clear();

		return data_ptr;
	}


	nanim_mesh::ptr resource_importer_manager::import_anim_mesh(const nstring& full_path)
	{
		return import_resource<nanim_mesh, nanim_mesh_importer>(full_path);
	}

	nstatic_mesh_lod::ptr resource_importer_manager::import_static_mesh(const nstring& full_path)
	{
		return import_resource<nstatic_mesh_lod, nstatic_mesh_importer>(full_path);
	}

	nskeletal_mesh::ptr resource_importer_manager::import_skeletal_mesh(const nstring& full_path)
	{
		return import_resource<nskeletal_mesh, nskeletal_mesh_importer>(full_path);
	}

	void resource_importer_manager::import_skeletal_anim(const nstring& full_path, nskeletal_anim_importer::data& out_data)
	{
		boost::filesystem::wpath fpath(full_path);

		if( !boost::filesystem::exists(fpath) 
			|| boost::filesystem::file_size(fpath) <= 0)
		{
			nthrow2(_T("mesh import failed - file not exists or empty"), full_path.c_str());
		}

		nstring ext = boost::filesystem::extension(fpath);
		boost::to_lower(ext);


		shared_ptr<nskeletal_anim_importer> importer;
		find_importer(ext, importer);

		if( !importer )
		{
			nLog_Error(_T("not find mesh importer for file : %s"), full_path.c_str());
			nthrow2(_T("mesh import failed"), full_path.c_str());
		}

		importer->import_from_file(full_path, out_data);
		importer->clear();
	}

	template<typename T>
	static shared_ptr<T> _find_importer_from_vector(std::vector<shared_ptr<T> >& importer_array, const nstring& file_type)
	{
		for(size_t i=0; i< importer_array.size(); i++)
		{
			shared_ptr<T> importer = importer_array[i];
			nstring imp_type = importer->get_file_type();

			if( _tcsicmp(imp_type.c_str(), file_type.c_str()) == 0 )
			{
				return importer;					 				
			}
		}

		return shared_ptr<T>();
	}

	void resource_importer_manager::find_importer(const nstring& file_type, nstatic_mesh_importer::ptr& out_ptr)
	{
		out_ptr = _find_importer_from_vector(m_static_importers, file_type);
	}

	void resource_importer_manager::find_importer(const nstring& file_type, nanim_mesh_importer::ptr& out_ptr)
	{
		out_ptr = _find_importer_from_vector(m_anim_importers, file_type);
	}

	void resource_importer_manager::find_importer(const nstring& file_type, nskeletal_mesh_importer::ptr& out_ptr)
	{
		out_ptr = _find_importer_from_vector(m_skel_mesh_importers, file_type);
	}

	void resource_importer_manager::find_importer(const nstring& file_type, nskeletal_anim_importer::ptr& out_ptr)
	{
		out_ptr = _find_importer_from_vector(m_skel_anim_importers, file_type);
	}

	template<typename T>
	static void _create_plugin_objects(std::vector<shared_ptr<T> >& obj_array)
	{
		const nclass* super_class = T::reflection_get_static_class();
		for(size_t i=0; i<super_class->get_num_children(); i++)
		{
			const nclass* sub_class = super_class->get_child(i);

			nobject* sub_obj = sub_class->alloc_object();
			if( sub_obj )
			{
				shared_ptr<T> importer(	static_cast<T*>(sub_obj) );
				obj_array.push_back(importer);
			}
		}//end of for()
	}

	void resource_importer_manager::load_plugin_classes()
	{
		//-- load sub classes of nstatic_mesh_importer
		_create_plugin_objects(m_static_importers);
		
		//-- load sub classes of nanim_mesh_importer
		_create_plugin_objects(m_anim_importers);		

		//-- load sub classes of nskeletal_mesh_importer
		_create_plugin_objects(m_skel_mesh_importers);		

		//-- load sub classes of nskeletal_anim_importer
		_create_plugin_objects(m_skel_anim_importers);		

		//-- report to log file
		wostringstream report;
		report << _T("importer now support : ");
		for(size_t i=0; i< m_static_importers.size(); i++)
		{
			nstatic_mesh_importer::ptr importer = m_static_importers[i];
			report << importer->get_file_type()
				<< _T(", ");
		}
		for(size_t i=0; i< m_anim_importers.size(); i++)
		{
			nanim_mesh_importer::ptr importer = m_anim_importers[i];
			report << importer->get_file_type()
				<< _T(", ");
		}
		for(size_t i=0; i< m_skel_mesh_importers.size(); i++)
		{
			nskeletal_mesh_importer::ptr importer = m_skel_mesh_importers[i];
			report << importer->get_file_type()
				<< _T(", ");
		}
		for(size_t i=0; i< m_skel_anim_importers.size(); i++)
		{
			nskeletal_anim_importer::ptr importer = m_skel_anim_importers[i];
			report << importer->get_file_type()
				<< _T(", ");
		}
		report << endl;
		nLog_Info(report.str().c_str());

	}

	void resource_importer_manager::destroy()
	{
		m_static_importers.clear();
		m_anim_importers.clear();
		m_skel_mesh_importers.clear();
		m_skel_anim_importers.clear();
	}
}//namespace nexus
