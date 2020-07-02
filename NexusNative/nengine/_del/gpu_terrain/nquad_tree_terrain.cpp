#include "stdafx.h"
#include <boost/bind.hpp>
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "nquad_tree_terrain.h"
#include "height_map_util.h"
#include "nterrain_actor.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nquad_tree_terrain, nprimitive_component)

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_chunk_vertex_data
	////////////////////////////////////////////////////////////////////////////
	nstring nterrain_chunk_vertex_data::get_vertex_type_name() const
	{
		return _T("terrain_chunk_vertex");
	}

	nstring nterrain_chunk_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_terrain");
	}

	void nterrain_chunk_vertex_data::init(size_t num_rows, size_t num_cols, bool enable_skirt)
	{
		size_t num_vert = num_rows*num_cols;
		
		if( enable_skirt )
		{
			// We are using a skirt along the edge, 
			// we get an additional border of vertices.
			num_vert += 2*num_rows + 2*num_cols - 4;
		}

		// alloc buffer
		init_pos_stream(num_vert);

		// fill the vertex buffer with the 'normal' vertices.
		vector3* pos_vert = reinterpret_cast<vector3*>( m_pos_stream.get_data() );
		const float inv_scale_x = 1.0f / (num_cols-1.0f);
		const float inv_scale_z = 1.0f / (num_rows-1.0f);

		float s, t;
		for (size_t i = 0; i < num_rows; i++) 
		{
			t = i * inv_scale_z;
			for (size_t j = 0; j < num_cols; j++) 
			{
				s = j * inv_scale_x;

				pos_vert->x = s;
				pos_vert->y = 1.0f;
				pos_vert->z = t;
				pos_vert++;
			}
		}// end of for

		if( enable_skirt )
		{
			// fill in the border/skirt vertices at the end of the vertex buffer.
			t = 0.0f;
			for (size_t j = 0; j < num_cols; j++) 
			{
				s = j * inv_scale_x;
				pos_vert->x = s;
				pos_vert->y = -1.0f;
				pos_vert->z = t;
				pos_vert++;
			}

			for (size_t i = 1; i < num_rows-1; i++) 
			{
				t = i * inv_scale_z;
				s = 0.0f;
				pos_vert->x = s;
				pos_vert->y = -1.0f;
				pos_vert->z = t;
				pos_vert++;

				s = 1.0f;
				pos_vert->x = s;
				pos_vert->y = -1.0f;
				pos_vert->z = t;
				pos_vert++;
			}

			t = 1.0f;
			for (size_t j = 0; j < num_cols; j++)
			{
				s = j * inv_scale_x;
				pos_vert->x = s;
				pos_vert->y = -1.0f;
				pos_vert->z = t;
				pos_vert++;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_chunk_render_mesh
	////////////////////////////////////////////////////////////////////////////
	nterrain_chunk_render_mesh::nterrain_chunk_render_mesh(void)
		: m_chunk_mesh(NULL),m_quad_factor(0,0,1)
	{}

	nterrain_chunk_render_mesh::~nterrain_chunk_render_mesh(void)
	{}

	void nterrain_chunk_render_mesh::draw_setup_effect(nshading_effect* effect_ptr)
	{
		effect_ptr->set_vector("TRN_QuadFactor", m_quad_factor);
		effect_ptr->set_vector("TRN_TexelSize", m_texel_size);
		effect_ptr->set_texture("TRN_HeightMap", m_heightmap_tex);
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nquad_tree_terrain
	////////////////////////////////////////////////////////////////////////////
	nquad_tree_terrain::nquad_tree_terrain(const nstring& name_str)
		: nprimitive_component(name_str),
		m_chunk_size(0),
		m_heightmap(NULL), m_heightmap_tex(NULL),
		m_enable_lod(true)
	{
		set_cast_dynamic_shadow(false);
		m_trans_flag.set(ETrans_Opaque);
	}

	nquad_tree_terrain::~nquad_tree_terrain(void)
	{
	}

	static size_t _get_max_lv(size_t chunk_size, size_t trn_size)
	{
		size_t pow2 = (trn_size-1) / (chunk_size-1);
		size_t lv = 0;
		for (size_t i = 1; i < pow2; i*=2)
			lv++;

		return lv;
	}

	void nquad_tree_terrain::create(nheight_map16* hmap, nrender_heightmap*	heightmap_tex, size_t chunk_size)
	{
		m_heightmap = hmap;
		m_heightmap_tex = heightmap_tex;
		m_chunk_size = chunk_size;
		m_max_lv = _get_max_lv(chunk_size, 
			std::min(hmap->get_width(), hmap->get_height())
			);

		//-- create render mesh
		create_shared_mesh();

		//-- init bounds
		update_bounds();		
	}

	void nquad_tree_terrain::create_shared_mesh()
	{
		//-- create mesh with skirt
		{
			//-- create memory data
			nterrain_chunk_vertex_data	vert_data;
			index_buffer16				index_data;

			vert_data.init(m_chunk_size, m_chunk_size, false);
			fill_indices_no_skirt(index_data);

			//-- create render data
			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

			m_chunk_mesh.reset( rres_mgr->alloc_static_mesh_indexed() );
			m_chunk_mesh->create(EDraw_TriangleStrip, &vert_data, &index_data, 0);
		}

		//-- create mesh without skirt
		{
			//-- create memory data
			nterrain_chunk_vertex_data	vert_data;
			index_buffer16				index_data;

			vert_data.init(m_chunk_size, m_chunk_size, false);
			fill_indices_no_skirt(index_data);

			//-- create render data
			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

			m_wireframe_mesh.reset( rres_mgr->alloc_static_mesh_indexed() );
			m_wireframe_mesh->create(EDraw_TriangleStrip, &vert_data, &index_data, 0);
		}		
	}	

	void nquad_tree_terrain::fill_indices_no_skirt(index_buffer16& index_data)
	{
		std::vector<unsigned short>& indices = index_data.data;

		unsigned short uiNumStrips = static_cast<unsigned short>(m_chunk_size-1);
		unsigned short uiLineStep = static_cast<unsigned short>(m_chunk_size);
		unsigned short uiStartVertex = 0;
		unsigned short uiStepX = 1;

		unsigned short uiNumIndices = 2*uiNumStrips*(m_chunk_size+1) - 2;

		indices.resize(uiNumIndices);
		unsigned short *p = &(*indices.begin());

		for (unsigned short j = 0; j < uiNumStrips; j++)
		{ 
			unsigned short uiVert = uiStartVertex;
			for (unsigned short k = 0; k < m_chunk_size; k++) 
			{
				*(p++) = uiVert;
				*(p++) = uiVert + uiLineStep;
				uiVert = uiVert + uiStepX;
			}
			uiStartVertex = uiStartVertex + uiLineStep;
			if (j+1 < uiNumStrips) // add degenerate triangle
			{
				*(p++) = (uiVert-uiStepX)+uiLineStep;
				*(p++) = uiStartVertex;
			}
		}
	}

	void nquad_tree_terrain::fill_indices(index_buffer16& index_data)
	{
		std::vector<unsigned short>& indices = index_data.data;

		unsigned short num_rows = static_cast<unsigned short>(m_chunk_size);
		unsigned short num_cols = static_cast<unsigned short>(m_chunk_size);
		unsigned short num_strips = num_rows-1;
		unsigned short line_step = num_cols;
		unsigned short start_vertex = 0;
		unsigned short step_x = 1;

		unsigned short uiNumStripIndices = 2*num_strips*(num_cols+1) - 2 + 2 + 4*num_cols * 2*num_strips + 2*(num_strips-1);

		indices.resize(uiNumStripIndices);
		unsigned short *p = &(*indices.begin());

		// regular grid
		for (unsigned short j = 0; j < num_strips; j++) 
		{ 
			unsigned short vert = start_vertex;
			for (unsigned short k = 0; k < num_cols; k++) 
			{
				*(p++) = vert;
				*(p++) = vert + line_step;
				vert = vert + step_x;
			}
			
			start_vertex = start_vertex + line_step;
			if (j+1 < num_strips) // add degenerate triangle
			{
				*(p++) = (vert-step_x)+line_step;
				*(p++) = start_vertex;
			}
		}

		// add degenerate triangles to start over
		start_vertex = start_vertex + line_step;
		*(p++) = *(p-1);
		*(p++) = start_vertex;

		// bottom border
		unsigned short skirt = start_vertex;
		for (unsigned short k = 0; k < num_cols; k++) 
		{
			*(p++) = skirt;
			*(p++) = k;
			skirt++;
		}

		// right border
		skirt++;
		unsigned short vert = num_cols + line_step - 1;
		for (unsigned short j = 0; j < num_strips-1; j++) 
		{
			*(p++) = skirt;
			*(p++) = vert;
			skirt += 2;
			vert = vert + line_step;
		}

		// top border, from right to left
		vert = num_rows*num_cols-1; 
		skirt =  vert + 2*line_step + 2*(line_step-2);
		for (unsigned short k = 0; k < num_cols; k++) 
		{
			*(p++) = skirt;
			*(p++) = vert;
			skirt--;
			vert--;
		}

		// left border, from top to bottom
		skirt = skirt - 1;
		vert = vert - line_step + 1;
		for (unsigned short j = 0; j < num_strips-1; j++) 
		{
			*(p++) = skirt;
			*(p++) = vert;
			skirt -= 2;
			vert = vert - line_step;
		}
		skirt = num_rows*num_cols;
		*(p++) = skirt;
		*(p++) = vert;
	}

	void nquad_tree_terrain::render(const nviewport& view)
	{
		m_render_nodes.clear();

		render_quad(0.0f, 0.0f, 1.0f, 1.0f, m_max_lv, 1.0f, view);				
	}

	void nquad_tree_terrain::render_quad(float minU, float minV, float maxU, float maxV, int level, float scale, const nviewport& view)
	{
		float halfU = (minU + maxU) / 2.0f;
		float halfV = (minV + maxV) / 2.0f;

		//-- frustum cull
		AABBox box;

		const vector3& map_scale = m_space.scale;		

		box.m_max.x = maxU*map_scale.x;
		box.m_max.y = 512*1024;
		box.m_max.z = maxV*map_scale.z;

		box.m_min.x = minU*map_scale.x;
		box.m_min.y = -512*1024;
		box.m_min.z = minV*map_scale.z;

		box.m_max += m_space.location;
		box.m_min += m_space.location;

		if( !view.frustum.intersect_box(box) )
			return;

		//--
		if ( !lod_split(minU, minV, maxU, maxV, view)
			|| level<1 )  
		{
			draw_mesh(minU, minV, scale, level);
		}
		else
		{
			scale = scale / 2.0f;
			render_quad(minU, minV, halfU, halfV, level-1, scale, view);
			render_quad(halfU, minV, maxU, halfV, level-1, scale, view);
			render_quad(minU, halfV, halfU, maxV, level-1, scale, view);
			render_quad(halfU, halfV, maxU, maxV, level-1, scale, view);
		}
	}

	void nquad_tree_terrain::draw_mesh(float uBias, float vBias, float scale, int level)
	{
		// 产生一个新的proxy对象
		trn_render_proxy::ptr proxy(m_render_pool.construct(),
			boost::bind(&nquad_tree_terrain::free_render_node, this, _1)
			);
		proxy->init( this );

		// 初始化mesh
		//if( nengine::instance()->get_render_mode() == ERM_Wireframe )
		{
			proxy->m_mesh.init(m_wireframe_mesh.get(), m_heightmap_tex);
		}
		//else
		//{
			//proxy->m_mesh.init(m_chunk_mesh.get(), m_heightmap_tex);
		//}

		//-- 设置mesh参数
		proxy->m_mesh.set_quad_factor(uBias, vBias, scale);
		proxy->m_mesh.set_texel_size(level, m_heightmap->get_width(), m_heightmap->get_height());

		m_render_nodes.push_back(proxy);

		//-- 发送至渲染器
		nrenderer_base* rnd = nengine::instance()->get_renderer();
		rnd->frame_object(proxy.get());
	}

	void nquad_tree_terrain::post_heightmap_change(const nrect& rc, nheight_map16* hmap)
	{
		m_heightmap_tex->copy_heightmap( rc, hmap );
		update_bounds();
	}

	nmaterial_base* nquad_tree_terrain::get_material() const
	{
		if( m_material )
			return m_material.get();
		else
			return nresource_manager::instance()->get_default_material();
	}

	void nquad_tree_terrain::create_material_basic(const resource_location& texture_loc)
	{
		shared_ptr<nterrain_mtl_basic> mtl( nNew nterrain_mtl_basic(_T("terrain_mtl")) );
		mtl->create(texture_loc);

		//--
		m_material = mtl;
	}

	bool nquad_tree_terrain::lod_split(float minU, float minV, float maxU, float maxV, const nviewport& view)
	{
		size_t w = m_heightmap->get_width();
		size_t h = m_heightmap->get_height();

		nrect rc(minU*w, minV*h, maxU*w, maxV*h);

		if(rc.get_width() <= m_chunk_size
			|| rc.get_height() <= m_chunk_size)
			return false;	

		if( !m_enable_lod )// 如果不使用LOD，则切分到最细
			return true;

		vector3 scale = m_space.scale;
		scale.x /= m_heightmap->get_width();
		scale.z /= m_heightmap->get_height();

		npoint cpt = rc.get_center();
		vector3 center(cpt.x, m_heightmap->get_value(cpt.x, cpt.y), cpt.y);
		center *= scale;

		float view_dist = vec_distance(center, view.camera.get_eye_pos());
		float edge_len = std::max(rc.get_width()*scale.x, rc.get_height()*scale.z);

#if 1
		return view_dist/edge_len < 2;
#else

		float min_resolution = 1.0f;
		float desired_resolution = 8.0f;

		//-- 计算d2
		float d2 = 0;

		int x_offset = rc.get_width()/2;
		int y_offset = rc.get_height()/2;

		int l = rc.left;
		int t = rc.top;
		int r = cpt.x + x_offset;
		int b = cpt.y + y_offset;

		int d_top = ( m_heightmap->get_value(l, t)+m_heightmap->get_value(r, t) )/2
			- m_heightmap->get_value(cpt.x, t);
		int d_right = ( m_heightmap->get_value(r, t)+m_heightmap->get_value(r, b) )/2
			- m_heightmap->get_value(r, cpt.y);
		int d_bottom = ( m_heightmap->get_value(l, b)+m_heightmap->get_value(r, b) )/2
			- m_heightmap->get_value(cpt.x, b);
		int d_left = ( m_heightmap->get_value(l, t)+m_heightmap->get_value(l, b) )/2
			- m_heightmap->get_value(l, cpt.y);
		int d_center = ( m_heightmap->get_value(l, t)+m_heightmap->get_value(r, b) )/2
			- m_heightmap->get_value(cpt.x, cpt.y);

		int d_max = std::max(abs(d_top), abs(d_right));
		d_max = std::max(abs(d_max), abs(d_bottom));
		d_max = std::max(abs(d_max), abs(d_left));
		d_max = std::max(abs(d_max), abs(d_center));

		d2 = d_max*m_scale.y/std::max(rc.get_width(), rc.get_height());		

		//-- 计算f
		float f = view_dist/( edge_len*min_resolution*std::max(desired_resolution*d2, 1.0f) );

		return f < 1.0f;
#endif
	}

	void nquad_tree_terrain::update_bounds()
	{		
		const vector3& scale = m_space.scale;
		vector3 pos = m_space.location;
		
		int max_raw = -1;
		int min_raw = std::numeric_limits<unsigned short>::max();
		unsigned short* raw = m_heightmap->get_raw_data();		
		for(int y=0;y<m_heightmap->get_height();y++)
		{
			for(int x=0;x<m_heightmap->get_width();x++)
			{
				if( *raw > max_raw )
					max_raw = *raw;
				if( *raw < min_raw )
					min_raw = *raw;
				raw++;
			}
		}// end of for
		

		float max_h = scale.y * max_raw+5;
		float min_h = scale.y * min_raw-5;
		
		float w = scale.x*0.5f;
		float h = scale.z*0.5f;

		pos.x += w;
		pos.y += (max_h+min_h)*0.5f;
		pos.z += h;

		m_bounds.box_extent = vector3(w, (max_h+min_h)*0.5f, h);
		m_bounds.origin = pos;
		vector3 pt = pos+m_bounds.box_extent;
		m_bounds.sphere_radius = vec_distance(pos, pt);
	}


	ntexture_splatting::ptr nquad_tree_terrain::create_texture_splatting(size_t alpha_w, size_t alpha_h)
	{
		ntexture_splatting::ptr new_mtl( nNew ntexture_splatting(_T("terrain_mtl")) );
		new_mtl->_init(alpha_w, alpha_h);

		m_material = new_mtl;

		return new_mtl;
	}

	void nquad_tree_terrain::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		nSERIALIZE(ar, m_material);
		nSERIALIZE(ar, m_trans_flag);
		nSERIALIZE(ar, m_enable_lod);
	}

	void nquad_tree_terrain::_update_transform(const object_space& parent_space)
	{
		m_space = parent_space;
		m_space.scale.x *= m_heightmap->get_width();
		m_space.scale.z *= m_heightmap->get_height();
		m_space.update_matrix();
		m_mat_world = m_space.local_to_world;

		update_bounds();
	}

	void nquad_tree_terrain::_level_loaded(nactor* owner)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(owner);
		nASSERT( trn != NULL );

		this->create(trn->get_height_map(),
			trn->get_height_map_texture(),
			trn->get_chunk_size()
			);

		nprimitive_component::_level_loaded(owner);
	}

	void nquad_tree_terrain::_destroy()
	{
		nprimitive_component::_destroy();

		m_heightmap = NULL;
		m_heightmap_tex = NULL;
		m_material.reset();
		m_chunk_mesh.reset();
		m_wireframe_mesh.reset();
		m_render_nodes.clear();
	}

	void nquad_tree_terrain::_on_device_lost(int param)
	{
		m_chunk_mesh.reset();
		m_wireframe_mesh.reset();
	}

	bool nquad_tree_terrain::_on_device_reset(int param)
	{
		create_shared_mesh();
		return true;
	}
}//namespace nexus