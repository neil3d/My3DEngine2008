#ifndef _NEXUS_STATIC_MESH_IMPORTER_BASE_H_
#define _NEXUS_STATIC_MESH_IMPORTER_BASE_H_
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../F3DFileHeader/StaticMeshFile.h"

namespace nexus
{
	struct vertex_traits
	{
		int pos_index,
			normal_index,
			tex_index;
	};
	inline bool operator < (const struct vertex_traits& t1, const struct vertex_traits& t2)
	{
		return memcmp(&t1, &t2, sizeof(vertex_traits)) < 0;
	}

	class static_mesh_base
	{
	public:
		static_mesh_base(void);
		virtual ~static_mesh_base(void);

	protected:
		void generate_tangent(nstatic_mesh_lod::ptr lod);
		box_sphere_bounds compute_bounding_box();
		nmesh_vertex_data::ptr create_vertex_data();

	protected:
		struct vertex
		{
			vector3	pos;
			vector3	normal;
			vector3 tangent;
			vector2	tex;
		};
		std::vector<vertex>				m_vertex_buffer;

		nstatic_mesh_lod::ptr _load_f3d_static_mesh(const F3D::StaticMeshHeader& header, FILE* fp);
	};
}//namespace nexus

#endif //_NEXUS_STATIC_MESH_IMPORTER_BASE_H_