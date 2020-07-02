#ifndef _NEXUS_RESOURCE_SKELETAL_MESH_H_
#define _NEXUS_RESOURCE_SKELETAL_MESH_H_

#include "nresource.h"
#include "../mesh/skeletal_mesh.h"

namespace nexus
{
	class nAPI nresource_skeletal_mesh :
		public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_skeletal_mesh> ptr;

		explicit nresource_skeletal_mesh(const nstring& name_str);
		virtual ~nresource_skeletal_mesh(void);

		void import_lod(const nstring& full_path);
		void import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr);

		virtual bool ready() const	{	return true;}
		virtual void serialize(narchive& ar);

		size_t get_num_lod() const	{	return m_lod_array.size(); }
		nskeletal_mesh::ptr get_lod_mesh(size_t lod) const	{	return m_lod_array[lod];}
		nmaterial_base* get_material(int lod, int mtl_id) const;
	
	private:
		std::vector<nskeletal_mesh::ptr>	m_lod_array;
		
		nDECLARE_NAMED_CLASS(nresource_skeletal_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_SKELETAL_MESH_H_