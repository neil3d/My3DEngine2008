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

		void import_lod(int lod, const nstring& full_path);
		void import_set_material(int lod, int sec, nmtl_base::ptr mtl_ptr);
		
		virtual void serialize(narchive& ar);
		virtual bool ready() const;

		size_t get_num_lod() const	{	return m_lod_array.size(); }
		size_t get_num_section(size_t lod) const;
		void get_lod_info(size_t i, int& num_vert, int& num_sec, int& num_tri) const;

		nskeletal_mesh::ptr get_lod_mesh(size_t lod) const	{	return m_lod_array[lod];}
		nmtl_base::ptr get_material(int lod, int mtl_id) const;
	
		void get_bounds(box_sphere_bounds& bounds) const;
	private:
		std::vector<nskeletal_mesh::ptr>	m_lod_array;
		
		nDECLARE_NAMED_CLASS(nresource_skeletal_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_SKELETAL_MESH_H_