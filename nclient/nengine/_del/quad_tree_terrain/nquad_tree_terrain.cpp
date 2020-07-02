#include "StdAfx.h"
#include "nquad_tree_terrain.h"
#include "../framework/nengine.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nquad_tree_terrain, nterrain_base)

	nquad_tree_terrain::nquad_tree_terrain(const nstring& name_str)
		: nterrain_base(name_str), m_cur_index(0)
	{
	}

	nquad_tree_terrain::~nquad_tree_terrain(void)
	{
	}

	void nquad_tree_terrain::create(size_t w, size_t h, short init_h, const terrain_cfg& cfg)
	{
		nterrain_base::create(w, h, init_h, cfg);

		m_matrix.create(w, h, false);

		//-- 分配最大所需的vertex和index buffer
		m_vert_data.init(w*h, 1, 1);		

		// 地形精度最高：w/2*h/2个quad tree node，每个node有4个三角形
		m_index_data.data.resize(w/2*h/2*4*3, 0);

		//-- 创建render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_render_mesh.reset( rres_mgr->alloc_dynamic_mesh_indexed() );
		m_render_mesh->create(EDraw_TriangleList, &m_vert_data, &m_index_data, 0);

		update_vertices(nrect(0,0, w, h));		
	}

	void nquad_tree_terrain::destroy()
	{
		nterrain_base::destroy();

		m_matrix.destroy();
		m_render_mesh.reset();
		m_index_data.data.clear();
	}

	void nquad_tree_terrain::update(float delta_time, const view_info& view)
	{
		nrect root_rc(0, 0, m_height_map.get_width()-1, m_height_map.get_height()-1);

		//-- update quad tree matrix
		if (m_cfg.enable_lod)
		{
			m_matrix.clear();
			update_matrix_r(root_rc, view);			
		}
		else
		{}

		//-- update mesh index data
		m_cur_index = 0;
		update_triangulation_r(root_rc, view);			
		m_render_mesh->update_indices(&m_index_data, m_cur_index);
	}

	bool nquad_tree_terrain::subdivision(const nrect& rc, const view_info& view) const
	{
		if(rc.get_width() <= 3
			|| rc.get_height() <= 3)
			return false;	

		npoint cpt = rc.get_center();
		vector3 center(cpt.x, m_height_map.get_value(cpt.x, cpt.y), cpt.y);
		center *= m_cfg.scale;
		
		float view_dist = vec_distance(center, view.cam->get_eye_pos());
		float edge_len = std::max(rc.get_width()*m_cfg.scale.x, rc.get_height()*m_cfg.scale.z);

		float min_resolution = 1.0f;
		float desired_resolution = 4.0f;

		//-- 计算d2
		float d2 = 0;

		int x_offset = rc.get_width()/2;
		int y_offset = rc.get_height()/2;

		int l = rc.left;
		int t = rc.top;
		int r = cpt.x + x_offset;
		int b = cpt.y + y_offset;

		int d_top = ( m_height_map.get_value(l, t)+m_height_map.get_value(r, t) )/2
			- m_height_map.get_value(cpt.x, t);
		int d_right = ( m_height_map.get_value(r, t)+m_height_map.get_value(r, b) )/2
			- m_height_map.get_value(r, cpt.y);
		int d_bottom = ( m_height_map.get_value(l, b)+m_height_map.get_value(r, b) )/2
			- m_height_map.get_value(cpt.x, b);
		int d_left = ( m_height_map.get_value(l, t)+m_height_map.get_value(l, b) )/2
			- m_height_map.get_value(l, cpt.y);
		int d_center = ( m_height_map.get_value(l, t)+m_height_map.get_value(r, b) )/2
			- m_height_map.get_value(cpt.x, cpt.y);

		int d_max = std::max(abs(d_top), abs(d_right));
		d_max = std::max(abs(d_max), abs(d_bottom));
		d_max = std::max(abs(d_max), abs(d_left));
		d_max = std::max(abs(d_max), abs(d_center));

		d2 = d_max*m_cfg.scale.y/std::max(rc.get_width(), rc.get_height());		

		//-- 计算f
		float f = view_dist/( edge_len*min_resolution*std::max(desired_resolution*d2, 1.0f) );
		
		return f < 1.0f;
	}

	static nrect _get_child_rect(const nrect& rc, int i)
	{
		int child_w = rc.get_width()/2;
		int child_h = rc.get_height()/2;

		/*
		-----------------
		|		|		|
		|	0	|	1	|
		|		|		|
		|---------------|
		|		|		|
		|	3	|	2	|
		|		|		|
		-----------------
		*/

		switch(i)
		{
		case 0:
			return nrect(rc.left,
				rc.top,
				rc.left+child_w,				
				rc.top+child_h);
		case 1:
			return nrect(rc.left+child_w,
				rc.top,
				rc.right,				
				rc.top+child_h);
		case 2:
			return nrect(rc.left+child_w,
				rc.top+child_h,
				rc.right,				
				rc.bottom);
		case 3:
			return nrect(rc.left,
				rc.top+child_h,
				rc.left+child_w,				
				rc.bottom);
		default:
			nASSERT(0 && "bad index");
		};
		return nrect(0,0,0,0);
	}

	bool nquad_tree_terrain::child_in_frustum(const nrect& rc, const nfrustum& frustum)
	{	

		AABBox box;
		
		box.m_max.x = rc.right*m_cfg.scale.x;
		box.m_max.y = 512*1024;
		box.m_max.z = rc.bottom*m_cfg.scale.z;

		box.m_min.x = rc.left*m_cfg.scale.x;
		box.m_min.y = -512*1024;
		box.m_min.z = rc.top*m_cfg.scale.z;

		return frustum.intersect_box(box);		
	}

	void nquad_tree_terrain::update_matrix_r(const nrect& rc, const view_info& view)
	{
		if( !child_in_frustum(rc, view.frustum) ) 
			return;

		if( subdivision(rc, view) )
		{
			npoint center = rc.get_center();
			m_matrix.set_value(center.x, center.y, true);

			//-- 向下细分			
			nrect child_rc;
			//-- child 0
			child_rc = _get_child_rect(rc, 0);
			update_matrix_r(child_rc, view);

			//-- child 1			
			child_rc = _get_child_rect(rc, 1);
			update_matrix_r(child_rc, view);

			//-- child 2			
			child_rc = _get_child_rect(rc, 2);
			update_matrix_r(child_rc, view);

			//-- child 3
			child_rc = _get_child_rect(rc, 3);
			update_matrix_r(child_rc, view);
		}
	}

	void nquad_tree_terrain::update_triangulation_r(const nrect& rc, const view_info& view)
	{
		if( !child_in_frustum(rc, view.frustum) ) 
			return;

		npoint center = rc.get_center();

		if( m_matrix.get_value(center.x, center.y) 
			&& rc.get_width() > 2
			&& rc.get_height() > 2
			)
		{
			//-- 向下细分			
			nrect child_rc;
			//-- child 0
			child_rc = _get_child_rect(rc, 0);
			update_triangulation_r(child_rc, view);

			//-- child 1			
			child_rc = _get_child_rect(rc, 1);
			update_triangulation_r(child_rc, view);

			//-- child 2			
			child_rc = _get_child_rect(rc, 2);
			update_triangulation_r(child_rc, view);

			//-- child 3
			child_rc = _get_child_rect(rc, 3);
			update_triangulation_r(child_rc, view);
		}
		else
		{
			//nLog_Info(_T("TRN, l=%d, t=%d, r=%d, b=%d\r\n"), rc.left, rc.top, rc.right, rc.bottom);
			std::vector<unsigned short> tri_fan;
			tri_fan.push_back( get_vert_index(center.x, center.y) );	// center point

			int w = rc.get_width();
			int h = rc.get_height();

			// top edge
			tri_fan.push_back( get_vert_index(rc.left, rc.top) );
			fix_top_r(nrect(rc.left, rc.top-h, rc.right, rc.bottom-h), tri_fan);
			tri_fan.push_back( get_vert_index(rc.right, rc.top) );

			// right edge
			fix_right_r(nrect(rc.left+w, rc.top, rc.right+w, rc.bottom), tri_fan);
			tri_fan.push_back( get_vert_index(rc.right, rc.bottom) );

			// bottom edge
			fix_bottom_r(nrect(rc.left, rc.top+h, rc.right, rc.bottom+h), tri_fan);
			tri_fan.push_back( get_vert_index(rc.left, rc.bottom) );

			// left edge
			fix_left_r(nrect(rc.left-w, rc.top, rc.right-w, rc.bottom), tri_fan);
			tri_fan.push_back( get_vert_index(rc.left, rc.top) );		

			// add to index buffer
			append_tri_fan(tri_fan);
		}
	}

	void nquad_tree_terrain::fix_top_r(const nrect& rc, std::vector<unsigned short>& tri_fan)
	{
		npoint center = rc.get_center();
		if( center.x < 0
			|| center.y < 0
			|| center.x >= m_height_map.get_width()
			|| center.y >= m_height_map.get_height())
			return;

		// 如果没有再细分，则返回
		if( !m_matrix.get_value(center.x, center.y) )
			return;

		// 测试相邻的子节点是否有细分
		fix_top_r( _get_child_rect(rc, 3), tri_fan );	// 左边的
		tri_fan.push_back( get_vert_index(center.x, rc.bottom) );
		fix_top_r( _get_child_rect(rc, 2), tri_fan );	// 右边的
	}

	void nquad_tree_terrain::fix_left_r(const nrect& rc, std::vector<unsigned short>& tri_fan)
	{
		npoint center = rc.get_center();
		if( center.x < 0
			|| center.y < 0
			|| center.x >= m_height_map.get_width()
			|| center.y >= m_height_map.get_height())
			return;

		// 如果没有再细分，则返回
		if( !m_matrix.get_value(center.x, center.y) )
			return;

		// 测试相邻的子节点是否有细分
		fix_left_r( _get_child_rect(rc, 2), tri_fan );	// 下边的
		tri_fan.push_back( get_vert_index(rc.right, center.y) );
		fix_left_r( _get_child_rect(rc, 1), tri_fan );	// 上边的
	}

	void nquad_tree_terrain::fix_right_r(const nrect& rc, std::vector<unsigned short>& tri_fan)
	{
		npoint center = rc.get_center();
		if( center.x < 0
			|| center.y < 0
			|| center.x >= m_height_map.get_width()
			|| center.y >= m_height_map.get_height())
			return;

		// 如果没有再细分，则返回
		if( !m_matrix.get_value(center.x, center.y) )
			return;

		// 测试相邻的子节点是否有细分
		fix_right_r( _get_child_rect(rc, 0), tri_fan );	// 上边的
		tri_fan.push_back( get_vert_index(rc.left, center.y) );
		fix_right_r( _get_child_rect(rc, 3), tri_fan );	// 下边的
	}

	void nquad_tree_terrain::fix_bottom_r(const nrect& rc, std::vector<unsigned short>& tri_fan)
	{
		npoint center = rc.get_center();
		if( center.x < 0
			|| center.y < 0
			|| center.x >= m_height_map.get_width()
			|| center.y >= m_height_map.get_height())
			return;

		// 如果没有再细分，则返回
		if( !m_matrix.get_value(center.x, center.y) )
			return;

		// 测试相邻的子节点是否有细分
		fix_bottom_r( _get_child_rect(rc, 1), tri_fan );	// 右边的
		tri_fan.push_back( get_vert_index(center.x, rc.top) );
		fix_bottom_r( _get_child_rect(rc, 0), tri_fan );	// 左边的
	}

	unsigned short nquad_tree_terrain::get_vert_index(int x, int y)
	{
		nASSERT( x >= 0 && x< m_height_map.get_width() );
		nASSERT( y >= 0 && y< m_height_map.get_height() );
		return (unsigned short)(m_height_map.get_width()*y+x);
	}

	void nquad_tree_terrain::append_tri_fan(const std::vector<unsigned short>& tri_fan)
	{
		nASSERT(m_cur_index+(tri_fan.size()-2)*3 <= m_index_data.get_index_count());

		size_t num = tri_fan.size();
		unsigned short shared = tri_fan[0];
		for(size_t i=2; i<num; i++)
		{
			m_index_data.data[m_cur_index++] = shared;
			m_index_data.data[m_cur_index++] = tri_fan[i-1];
			m_index_data.data[m_cur_index++] = tri_fan[i];
		}
	}
	
	void nquad_tree_terrain::update_vertices(const nrect& rc)
	{
		vertex_stream* pos_stream = m_vert_data.get_stream(0);
		vertex_stream* sec_stream = m_vert_data.get_stream(1);

		vector3* pos_vert = (vector3*)pos_stream->get_data();

		int w = m_height_map.get_width();		
		const vector3& scale = m_cfg.scale;

		for(int y=rc.top; y<rc.bottom; y++)
		{
			int line_base = y*w;
			for(int x=rc.left; x<rc.right; x++)
			{
				vector3& pos = pos_vert[line_base+x];
				pos.x = x * scale.x;
				pos.y = m_height_map.get_value(x, y) * scale.y;
				pos.z = y * scale.z;
			}
		}// end of for

		m_render_mesh->update_vertices(&m_vert_data);
	}
}//namespace nexus