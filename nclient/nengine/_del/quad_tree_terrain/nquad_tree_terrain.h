#ifndef _NEXUS_QUAD_TREE_TERRAIN_H_
#define _NEXUS_QUAD_TREE_TERRAIN_H_
#include "nterrain_base.h"
#include "nbit_map.h"
#include "../mesh/mesh_vertex_data_common.h"
#include "../mesh/mesh_index_data.h"
#include "../renderer/nrender_dynamic_mesh_indexed.h"

namespace nexus
{
	class nquad_tree_terrain
		: public nterrain_base
	{
	public:
		nquad_tree_terrain(const nstring& name_str);
		virtual ~nquad_tree_terrain(void);

		virtual void create(size_t w, size_t h, short init_h, const terrain_cfg& cfg);
		virtual void destroy();

		virtual nrender_mesh* get_render_mesh(int lod)	
		{
			(void)lod;
			return m_render_mesh.get();
		}

		virtual void update(float delta_time, const view_info& view);

	protected:
		virtual void post_height_changed(const nrect& rc)
		{
			update_vertices(rc);
		}

	private:
		// 更新quad tree matrix，然后更新index bufer
		void update_triangulation_r(const nrect& rc, const view_info& view);
		void append_tri_fan(const std::vector<unsigned short>& tri_fan);
		unsigned short get_vert_index(int x, int y);
		void fix_top_r(const nrect& rc, std::vector<unsigned short>& tri_fan);
		void fix_left_r(const nrect& rc, std::vector<unsigned short>& tri_fan);
		void fix_right_r(const nrect& rc, std::vector<unsigned short>& tri_fan);
		void fix_bottom_r(const nrect& rc, std::vector<unsigned short>& tri_fan);

		void update_matrix_r(const nrect& rc, const view_info& view);
		bool subdivision(const nrect& rc, const view_info& view) const;
		// 根据dirty mark，更新vertex buffer
		void update_vertices(const nrect& rc);

		bool child_in_frustum(const nrect& rc, const nfrustum& frustum);
	private:
		nbit_map	m_matrix;
		
		render_res_ptr<nrender_dynamic_mesh_indexed>
									m_render_mesh;
		nmesh_vertex_data_common	m_vert_data;
		index_buffer16				m_index_data;

		//-- update
		size_t	m_cur_index;

		nDECLARE_NAMED_CLASS(nquad_tree_terrain)
	};
}//namespace nexus

#endif //_NEXUS_QUAD_TREE_TERRAIN_H_