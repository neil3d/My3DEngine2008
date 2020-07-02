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

		virtual nmaterial_base* get_material(int lod, int mtl_id) const;

		virtual bool ready() const
		{
			// todo
			return true;
		}

		void import_mesh(const nstring& full_path);
		void import_set_material(int mtl_id, nmaterial_base::ptr mtl_ptr);

		virtual void serialize(narchive& ar)
		{
			nresource::serialize(ar);
			nSERIALIZE(ar, m_mesh);
		}

		nanim_mesh::ptr get_mesh_data() const	{	return m_mesh;}
	private:
		nanim_mesh::ptr	m_mesh;

		nDECLARE_NAMED_CLASS(nresource_anim_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_ANIM_MESH_H_