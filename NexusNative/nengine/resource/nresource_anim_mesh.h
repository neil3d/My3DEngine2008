#ifndef _NEXUS_RESOURCE_ANIM_MESH_H_
#define _NEXUS_RESOURCE_ANIM_MESH_H_
#include "nresource.h"
#include "../mesh/anim_mesh.h"
#include "../renderer/nrender_anim_mesh.h"

namespace nexus
{
	/**
	 *	顶点关键帧动画
	 *	@remark 考虑到顶点动画比较费容量，所以不支持LOD
	*/
	class nAPI nresource_anim_mesh :
		public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_anim_mesh> ptr;

		explicit nresource_anim_mesh(const nstring& name_str);
		virtual ~nresource_anim_mesh(void);

		virtual nmaterial_base::ptr get_material(int lod, int mtl_id) const;

		virtual bool ready() const
		{
			// todo
			return true;
		}

		void import_lod(int lod, const nstring& full_path);
		void import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr);

		virtual void serialize(narchive& ar)
		{
			nresource::serialize(ar);
			nSERIALIZE(ar, m_lod_array);
		}

		nanim_mesh::ptr get_lod_mesh(int lod) const	
		{	
			nASSERT(lod>=0 && lod<(int)m_lod_array.size());
			return m_lod_array[lod];
		}
		size_t get_num_lod() const	{	return m_lod_array.size();}

		size_t get_num_section(size_t lod) const;
		void get_lod_info(size_t i, int& num_vert, int& num_sec, int& num_tri) const;

		size_t get_num_sequence() const;
		void get_sequence_info(size_t i, nstring& name, float& length) const;
	private:
		typedef std::vector<nanim_mesh::ptr> st_lod_array;
		st_lod_array	m_lod_array;

		nDECLARE_NAMED_CLASS(nresource_anim_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_ANIM_MESH_H_