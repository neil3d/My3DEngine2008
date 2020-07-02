/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_RESOURCE_IMPORTER_MANAGER_H_
#define _NEXUS_RESOURCE_IMPORTER_MANAGER_H_
#include "../mesh//nmesh_importer.h"

namespace nexus
{
	/**
	 * 资源导入插件管理
	 * @目前支持：静态模型导入;morph动画导入;skeletal mesh/anim导入
	*/
	class resource_importer_manager
	{
	public:
		resource_importer_manager(void);
		~resource_importer_manager(void);

		static resource_importer_manager* instance();
		void load_plugin_classes();
		void destroy();

		nstatic_mesh_lod::ptr import_static_mesh(const nstring& full_path);
		nanim_mesh::ptr import_anim_mesh(const nstring& full_path);
		nskeletal_mesh::ptr import_skeletal_mesh(const nstring& full_path);
		void import_skeletal_anim(const nstring& full_path, nskeletal_anim_importer::data& out_data);

		size_t get_num_static_mesh_importer() const		{	return m_static_importers.size();}
		nstring get_static_mesh_importer_type(size_t i) const	{	return m_static_importers[i]->get_file_type(); }
		nstring get_static_mesh_importer_desc(size_t i) const	{	return m_static_importers[i]->get_desc(); }

	private:
		template<typename TData, typename TImporter>
		shared_ptr<TData> import_resource(const nstring& full_path);

		void find_importer(const nstring& file_type, nstatic_mesh_importer::ptr& out_ptr);
		void find_importer(const nstring& file_type, nanim_mesh_importer::ptr& out_ptr);
		void find_importer(const nstring& file_type, nskeletal_mesh_importer::ptr& out_ptr);
		void find_importer(const nstring& file_type, nskeletal_anim_importer::ptr& out_ptr);

		std::vector<nstatic_mesh_importer::ptr>	m_static_importers;
		std::vector<nanim_mesh_importer::ptr>	m_anim_importers;
		
		std::vector<nskeletal_mesh_importer::ptr>	m_skel_mesh_importers;
		std::vector<nskeletal_anim_importer::ptr>	m_skel_anim_importers;
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_IMPORTER_MANAGER_H_